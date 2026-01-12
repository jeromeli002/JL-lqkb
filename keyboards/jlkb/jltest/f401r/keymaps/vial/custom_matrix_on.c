#include "quantum.h"
#include "jlrgb.h"
#include "tm1640.h"
#include "gpio.h"
#include "eeprom.h"
#include <string.h>

// ========================== 1. RGB 全局数据定义 ==========================
// 修复链接错误：确保自定义灯效引用的变量在本项目中定义
remote_rgb_data_t g_remote_rgb_data = {
    .h = 79,
    .s = 255,
    .v = 25,
    .spd = 255
};

// ========================== 2. 指示灯配置结构与 EEPROM ==========================
typedef struct {
    uint8_t index; // 灯珠索引
    uint8_t count; // 灯珠数量
    uint8_t h;     // 色调
    uint8_t s;     // 饱和度
    uint8_t v;     // 亮度
} led_cfg_t;

typedef struct {
    led_cfg_t caps;
    led_cfg_t num;
    led_cfg_t scrl;
    uint8_t magic; 
} indicator_config_t;

// 校验码：用于判定 EEPROM 数据是否有效
#define INDICATOR_MAGIC 0x8A 
// 在 F401/Vial 环境下，建议使用较大的偏移量以防与系统配置冲突
#define EEPROM_INDICATOR_ADDR 64

indicator_config_t g_ind_cfg;

// ========================== 3. TM1640 宏定义与声明 ==========================
#define MATRIX_LIGHT_ROWS TM1640_ROWS
#define MATRIX_LIGHT_COLS TM1640_COLS

void handle_external_bitmap_data(const uint8_t *data, uint16_t length, tm1640_brightness_t brightness);

// ========================== 4. 初始化流程 ==========================

void matrix_init_kb(void) {
    // TM1640 初始化
    tm1640_init();
    tm1640_start_blink(); 

    // 从 EEPROM 读取配置
    eeprom_read_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    
    // 如果 Magic 不匹配，说明是首次上电，加载默认值
    if (g_ind_cfg.magic != INDICATOR_MAGIC) {
        // 默认值：Caps(红,Idx 0), Num(绿,Idx 1), Scrl(蓝,Idx 2)
        g_ind_cfg.caps = (led_cfg_t){0, 1, 0, 255, 255};
        g_ind_cfg.num  = (led_cfg_t){1, 1, 85, 255, 255};
        g_ind_cfg.scrl = (led_cfg_t){2, 1, 170, 255, 255};
        
        g_ind_cfg.magic = INDICATOR_MAGIC;
        // 立即同步到 EEPROM
        eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    }

    matrix_init_user();
}

void keyboard_post_init_user(void) {
    // 默认进入自定义灯效模式 0
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_mode0);
}

// ========================== 5. 主循环：解决闪烁与常亮 (核心修复) ==========================
void matrix_scan_kb(void) {
    tm1640_task(); 

#ifdef RGBLIGHT_ENABLE
    led_t led_state = host_keyboard_led_state();
    static led_t last_led_state;

    // --- Caps Lock 强制刷新 ---
    if (led_state.caps_lock) {
        rgblight_sethsv_range(g_ind_cfg.caps.h, g_ind_cfg.caps.s, g_ind_cfg.caps.v, 
                              g_ind_cfg.caps.index, g_ind_cfg.caps.index + g_ind_cfg.caps.count);
    } else if (last_led_state.caps_lock) {
        for (uint8_t i = g_ind_cfg.caps.index; i < g_ind_cfg.caps.index + g_ind_cfg.caps.count; i++) 
            rgblight_sethsv_at(0, 0, 0, i);
    }

    // --- Num Lock 强制刷新 ---
    if (led_state.num_lock) {
        rgblight_sethsv_range(g_ind_cfg.num.h, g_ind_cfg.num.s, g_ind_cfg.num.v, 
                              g_ind_cfg.num.index, g_ind_cfg.num.index + g_ind_cfg.num.count);
    } else if (last_led_state.num_lock) {
        for (uint8_t i = g_ind_cfg.num.index; i < g_ind_cfg.num.index + g_ind_cfg.num.count; i++) 
            rgblight_sethsv_at(0, 0, 0, i);
    }

    // --- Scroll Lock 强制刷新 ---
    if (led_state.scroll_lock) {
        rgblight_sethsv_range(g_ind_cfg.scrl.h, g_ind_cfg.scrl.s, g_ind_cfg.scrl.v, 
                              g_ind_cfg.scrl.index, g_ind_cfg.scrl.index + g_ind_cfg.scrl.count);
    } else if (last_led_state.scroll_lock) {
        for (uint8_t i = g_ind_cfg.scrl.index; i < g_ind_cfg.scrl.index + g_ind_cfg.scrl.count; i++) 
            rgblight_sethsv_at(0, 0, 0, i);
    }

    last_led_state = led_state;
#endif

    matrix_scan_user();
}

// ========================== 6. HID 接收回调 (完整指令集) ==========================
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (data[0] != 0xAB) return;

    switch (data[1]) {
        case 0xA0: tm1640_display_off(); break;
        case 0xA1: handle_external_bitmap_data(data, length, TM1640_DEFAULT_BRIGHTNESS_CMD); break;
        case 0xA2: bootloader_jump(); break;
        case 0xA3: eeconfig_init(); wait_ms(200); soft_reset_keyboard(); break;
        case 0xA4: soft_reset_keyboard(); break;
        case 0xA5: tm1640_start_running_light(); break;
        case 0xA6: tm1640_start_blink(); break;
        case 0xA7: setPinInput(C1); break;
        case 0xA8: setPinOutput(C1); writePinLow(C1); break;
        case 0xA9: setPinInput(B5); break;
        case 0xAA: setPinOutput(B5); writePinLow(B5); break;
            
        case 0xB0: // RGB 全局控制
            g_remote_rgb_data.h = data[2]; 
            g_remote_rgb_data.s = data[3]; 
            g_remote_rgb_data.v = data[4]; 
            g_remote_rgb_data.spd = data[5]; 
            if (length >= 6 + MAX_LED_BYTE_COUNT) {
                memcpy(g_remote_rgb_data.led_bitmap, &data[6], MAX_LED_BYTE_COUNT);
            }
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
            break;
        case 0xB1: rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_off); break;
        case 0xB2: 
            g_remote_rgb_data.h = 214; g_remote_rgb_data.s = 255; g_remote_rgb_data.v = 255;
            g_remote_rgb_data.led_bitmap[0] = 0xA0;
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
            break;

        // --- 指示灯自定义指令 ---
        case 0xC0: // Caps 预览
            g_ind_cfg.caps.index = data[2]; g_ind_cfg.caps.count = data[3];
            g_ind_cfg.caps.h = data[4]; g_ind_cfg.caps.s = data[5]; g_ind_cfg.caps.v = data[6];
            break;
        case 0xC1: // Caps 保存
            g_ind_cfg.magic = INDICATOR_MAGIC;
            eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
            break;

        case 0xC2: // Num 预览
            g_ind_cfg.num.index = data[2]; g_ind_cfg.num.count = data[3];
            g_ind_cfg.num.h = data[4]; g_ind_cfg.num.s = data[5]; g_ind_cfg.num.v = data[6];
            break;
        case 0xC3: // Num 保存
            g_ind_cfg.magic = INDICATOR_MAGIC;
            eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
            break;

        case 0xC4: // Scrl 预览
            g_ind_cfg.scrl.index = data[2]; g_ind_cfg.scrl.count = data[3];
            g_ind_cfg.scrl.h = data[4]; g_ind_cfg.scrl.s = data[5]; g_ind_cfg.scrl.v = data[6];
            break;
        case 0xC5: // Scrl 保存
            g_ind_cfg.magic = INDICATOR_MAGIC;
            eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
            break;

        default: break;
    }
}

// ========================== 7. TM1640 处理函数 ==========================
void handle_external_bitmap_data(const uint8_t *data, uint16_t length, tm1640_brightness_t brightness) {
    if (MATRIX_LIGHT_ROWS > 8) return;
    const uint8_t BYTES_PER_ROW = (MATRIX_LIGHT_COLS + 7) / 8;
    const uint16_t EXPECTED_DATA_BYTES = 2 + (uint16_t)MATRIX_LIGHT_ROWS * BYTES_PER_ROW;

    if (length < EXPECTED_DATA_BYTES) return;

    uint8_t tm1640_col_data[MATRIX_LIGHT_COLS];
    memset(tm1640_col_data, 0, MATRIX_LIGHT_COLS);
    
    const uint8_t *matrix_data = data + 2;

    for (uint8_t row = 0; row < MATRIX_LIGHT_ROWS; row++) {
        const uint8_t *current_row_data = matrix_data + (uint16_t)row * BYTES_PER_ROW;
        for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
            if (current_row_data[col / 8] & (1 << (col % 8))) {
                tm1640_col_data[col] |= (1 << row);
            }
        }
    }
    tm1640_display_bitmap(tm1640_col_data, brightness);
}

// STM32F401 不需要 AFIO
void board_init(void) {}

// 保持为空，逻辑已移至 matrix_scan 以实现强制常亮
bool led_update_kb(led_t led_state) {
    return led_update_user(led_state);
}