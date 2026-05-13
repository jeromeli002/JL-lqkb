#include "quantum.h"
#include "jlrgb.h"
#include "tm1640.h"
#include "gpio.h"
#include "raw_hid.h"
#include "eeprom.h"
#include <string.h>

// ========================== 1. 数据结构与全局变量 ==========================
remote_rgb_data_t g_remote_rgb_data = {
    .h = 79, .s = 255, .v = 25, .spd = 255
};

typedef struct {
    uint8_t index; uint8_t count; uint8_t h; uint8_t s; uint8_t v;
} led_cfg_t;

typedef struct {
    led_cfg_t caps;
    led_cfg_t num;
    led_cfg_t scrl;
    led_cfg_t layers[16];
    uint32_t rgb_timeout; // RGB 自动休眠时间（单位：秒，0为不关闭） 
    uint8_t magic; 
} indicator_config_t;

#define INDICATOR_MAGIC 0x8E 
#define EEPROM_INDICATOR_ADDR 4096

indicator_config_t g_ind_cfg;

// 用于动态处理 RGB 超时的全局状态变量
static uint32_t custom_last_activity_time = 0;
static bool is_rgb_timeout_sleep = false;

// ========================== 2. TM1640 宏与声明 ==========================
#define MATRIX_LIGHT_ROWS TM1640_ROWS
#define MATRIX_LIGHT_COLS TM1640_COLS
void handle_external_bitmap_data(const uint8_t *data, uint16_t length, tm1640_brightness_t brightness);

// ========================== 3. 初始化 ==========================
void matrix_init_kb(void) {
    tm1640_init();
    tm1640_start_blink(); 

    eeprom_read_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    
    if (g_ind_cfg.magic != INDICATOR_MAGIC) {
        g_ind_cfg.caps = (led_cfg_t){0, 1, 0, 255, 255};
        g_ind_cfg.num  = (led_cfg_t){1, 1, 85, 255, 255};
        g_ind_cfg.scrl = (led_cfg_t){2, 1, 170, 255, 255};
        for(uint8_t i=0; i<16; i++) {
            g_ind_cfg.layers[i] = (led_cfg_t){(uint8_t)(27-i), 1, 128, 255, 255};
        }
        g_ind_cfg.rgb_timeout = 180; // 默认 180 秒
        g_ind_cfg.magic = INDICATOR_MAGIC;
        eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    }  
    matrix_init_user();  
}

void keyboard_post_init_user(void) {
    // 启用自定义静态模式
    // rgb_matrix_mode(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
    setPinInput(C1);
    // setPinOutput(C1);
    // writePinHigh(C1);
}

// ========================== 4. 主循环 (仅保留 TM1640) ==========================
void matrix_scan_kb(void) {
    tm1640_task(); 
    matrix_scan_user();
}

void matrix_scan_user(void) {
    // --- 1. RGB 动态超时休眠逻辑 ---
    if (g_ind_cfg.rgb_timeout > 0) {
        // 判断超过设置的秒数 (乘以1000转换为毫秒)
        if (!is_rgb_timeout_sleep && timer_elapsed32(custom_last_activity_time) > (g_ind_cfg.rgb_timeout * 1000UL)) {
            is_rgb_timeout_sleep = true;
            rgb_matrix_disable_noeeprom(); // 关闭 RGB 矩阵以省电
        }
    } else {
        // 设置为0 (永不休眠) 时的唤醒保护
        if (is_rgb_timeout_sleep) {
            is_rgb_timeout_sleep = false;
            rgb_matrix_enable_noeeprom();
        }
    }
  }
    
// ========================== 5. RGB 指示灯核心逻辑 (最高优先级) ==========================
// 此函数在每一帧渲染最后执行，确保指示灯常亮且不被特效覆盖
bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) return false;

    // --- 5.1 处理层灯 (较低优先级) ---
    for (uint8_t i = 0; i < 16; i++) {
        if (IS_LAYER_ON_STATE(layer_state, i)) {
            RGB hsv = hsv_to_rgb((HSV){g_ind_cfg.layers[i].h, g_ind_cfg.layers[i].s, g_ind_cfg.layers[i].v});
            for (uint8_t n = 0; n < g_ind_cfg.layers[i].count; n++) {
                rgb_matrix_set_color(g_ind_cfg.layers[i].index + n, hsv.r, hsv.g, hsv.b);
            }
        }
    }

    // --- 5.2 处理系统指示灯 (最高优先级，覆盖层灯) ---
    led_t led_state = host_keyboard_led_state();

    if (led_state.caps_lock) {
        RGB hsv = hsv_to_rgb((HSV){g_ind_cfg.caps.h, g_ind_cfg.caps.s, g_ind_cfg.caps.v});
        for (uint8_t n = 0; n < g_ind_cfg.caps.count; n++) {
            rgb_matrix_set_color(g_ind_cfg.caps.index + n, hsv.r, hsv.g, hsv.b);
        }
    }

    if (led_state.num_lock) {
        RGB hsv = hsv_to_rgb((HSV){g_ind_cfg.num.h, g_ind_cfg.num.s, g_ind_cfg.num.v});
        for (uint8_t n = 0; n < g_ind_cfg.num.count; n++) {
            rgb_matrix_set_color(g_ind_cfg.num.index + n, hsv.r, hsv.g, hsv.b);
        }
    }

    if (led_state.scroll_lock) {
        RGB hsv = hsv_to_rgb((HSV){g_ind_cfg.scrl.h, g_ind_cfg.scrl.s, g_ind_cfg.scrl.v});
        for (uint8_t n = 0; n < g_ind_cfg.scrl.count; n++) {
            rgb_matrix_set_color(g_ind_cfg.scrl.index + n, hsv.r, hsv.g, hsv.b);
        }
    }

    return true;
}

// ========================== 6. RAW HID 指令集 ==========================
//===== 原生qmk via 使用 via_command_kb ======
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (length < 2 || data[0] != 0xAB) return;

    switch (data[1]) {
        
        case 0x00: clear_keyboard();bootloader_jump(); break;
        case 0x01: eeconfig_init(); wait_ms(200); soft_reset_keyboard(); break;
        case 0x02: soft_reset_keyboard(); break;
        
        case 0x10: tm1640_display_off(); break;
        case 0x11: handle_external_bitmap_data(data, length, TM1640_DEFAULT_BRIGHTNESS_CMD); break;
        case 0x12: tm1640_start_running_light(); break;
        case 0x13: tm1640_start_blink(); break;
        
        case 0x20: setPinInput(C1); break;
        case 0x21: setPinOutput(C1); writePinLow(C1); break;
        case 0x22: setPinInput(B5); break;
        case 0x23: setPinOutput(B5); writePinLow(B5); break;
        
        case 0x90: 
            g_remote_rgb_data.h = data[2]; g_remote_rgb_data.s = data[3]; 
            g_remote_rgb_data.v = data[4]; g_remote_rgb_data.spd = data[5]; 
            uint8_t copy_len = (length - 6 < MAX_LED_BYTE_COUNT) ? (length - 6) : MAX_LED_BYTE_COUNT;
            memcpy(g_remote_rgb_data.led_bitmap, &data[6], copy_len);
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
            break;
        
        case 0x91: rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_off); break;
        
        case 0x92: 
            g_remote_rgb_data.h = 214; g_remote_rgb_data.s = 255; g_remote_rgb_data.v = 255;
            g_remote_rgb_data.led_bitmap[0] = 0xA0;
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
            break;

        case 0x80: // Save 指令保持独立，便于统一下发保存
            g_ind_cfg.magic = INDICATOR_MAGIC;
            eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
            break;
            
        case 0x81: // 返回(读取)指示灯及休眠配置
        {
            // 创建响应包，默认填充为 0
            uint8_t resp[32]; // QMK Raw HID 常规包长为 32，你可以用 VLA: uint8_t resp[length] 或定长处理
            memset(resp, 0, sizeof(resp));
            
            resp[0] = 0xAB;     // Magic Byte
            resp[1] = 0x81;     // 返回指令标识
            resp[2] = data[2];  // 对应的子类型 (00, 01, 02, 03, 1X)

            if (data[2] == 0x00) {
                // 读取 Caps 指示灯
                resp[3] = g_ind_cfg.caps.index; resp[4] = g_ind_cfg.caps.count;
                resp[5] = g_ind_cfg.caps.h;     resp[6] = g_ind_cfg.caps.s;     resp[7] = g_ind_cfg.caps.v;
            } 
            else if (data[2] == 0x01) {
                // 读取 Num 指示灯
                resp[3] = g_ind_cfg.num.index;  resp[4] = g_ind_cfg.num.count;
                resp[5] = g_ind_cfg.num.h;      resp[6] = g_ind_cfg.num.s;      resp[7] = g_ind_cfg.num.v;
            } 
            else if (data[2] == 0x02) {
                // 读取 Scrl 指示灯
                resp[3] = g_ind_cfg.scrl.index; resp[4] = g_ind_cfg.scrl.count;
                resp[5] = g_ind_cfg.scrl.h;     resp[6] = g_ind_cfg.scrl.s;     resp[7] = g_ind_cfg.scrl.v;
            } 
            else if (data[2] == 0x03) {
                // 读取动态 RGB 休眠时间（将32位拆分成4个字节返回）
                resp[3] = (g_ind_cfg.rgb_timeout >> 24) & 0xFF;
                resp[4] = (g_ind_cfg.rgb_timeout >> 16) & 0xFF;
                resp[5] = (g_ind_cfg.rgb_timeout >> 8)  & 0xFF;
                resp[6] =  g_ind_cfg.rgb_timeout        & 0xFF;
            } 
            else if (data[2] >= 0x10 && data[2] <= 0x1F) {
                // 读取层指示灯配置
                uint8_t layer_idx = data[2] - 0x10;
                resp[3] = g_ind_cfg.layers[layer_idx].index;
                resp[4] = g_ind_cfg.layers[layer_idx].count;
                resp[5] = g_ind_cfg.layers[layer_idx].h;
                resp[6] = g_ind_cfg.layers[layer_idx].s;
                resp[7] = g_ind_cfg.layers[layer_idx].v;
            }
            
            // 将数据发回主机
            raw_hid_send(resp, length);
            break;
        }

        case 0x82: // 统一指示灯及休眠配置 (基于 data[2] 区分)
            if (data[2] == 0x00) { 
                // Caps 指示灯
                g_ind_cfg.caps.index = data[3]; g_ind_cfg.caps.count = data[4];
                g_ind_cfg.caps.h = data[5]; g_ind_cfg.caps.s = data[6]; g_ind_cfg.caps.v = data[7];
            } 
            else if (data[2] == 0x01) { 
                // Num 指示灯
                g_ind_cfg.num.index = data[3]; g_ind_cfg.num.count = data[4];
                g_ind_cfg.num.h = data[5]; g_ind_cfg.num.s = data[6]; g_ind_cfg.num.v = data[7];
            } 
            else if (data[2] == 0x02) { 
                // Scrl 指示灯
                g_ind_cfg.scrl.index = data[3]; g_ind_cfg.scrl.count = data[4];
                g_ind_cfg.scrl.h = data[5]; g_ind_cfg.scrl.s = data[6]; g_ind_cfg.scrl.v = data[7];
            } 
            else if (data[2] == 0x03) { 
                // 设置动态 RGB 休眠时间（数据向后偏移，读取 data[3] ~ data[6]）
                g_ind_cfg.rgb_timeout = ((uint32_t)data[3] << 24) | 
                                        ((uint32_t)data[4] << 16) | 
                                        ((uint32_t)data[5] << 8)  | 
                                         (uint32_t)data[6];
                
                // 休眠配置直接保存至 EEPROM
                g_ind_cfg.magic = INDICATOR_MAGIC;
                eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
                
                custom_last_activity_time = timer_read32();
                if (is_rgb_timeout_sleep && g_ind_cfg.rgb_timeout > 0) {
                    is_rgb_timeout_sleep = false;
                    rgb_matrix_enable_noeeprom();
                }
            } 
            else if (data[2] >= 0x10 && data[2] <= 0x1F) { 
                // 层指示灯配置 (10对应层0, 11对应层1 ... 1F对应层15)
                uint8_t layer_idx = data[2] - 0x10;
                g_ind_cfg.layers[layer_idx].index = data[3];
                g_ind_cfg.layers[layer_idx].count = data[4];
                g_ind_cfg.layers[layer_idx].h     = data[5];
                g_ind_cfg.layers[layer_idx].s     = data[6];
                g_ind_cfg.layers[layer_idx].v     = data[7];
            }
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

void board_init(void) {}
bool led_update_kb(led_t led_state) { return led_update_user(led_state); }