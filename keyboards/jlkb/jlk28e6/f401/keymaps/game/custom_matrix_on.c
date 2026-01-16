#include "quantum.h"
#include "eeprom.h"
#include <string.h>
#include "jlrgb.h"

// ===============================OLED屏幕初始化=============================
void jloled_init(void) {
jloled_display_slot(0); //初始化第一槽位图像
}

// ========================== 1. 数据结构与全局变量 ==========================
remote_rgb_data_t g_remote_rgb_data = {
    .h = 79, .s = 255, .v = 25, .spd = 255
};

typedef struct {
    uint8_t index; uint8_t count; uint8_t h; uint8_t s; uint8_t v;
} led_cfg_t;

typedef struct {
    led_cfg_t layers[16]; 
    uint8_t magic; 
} indicator_config_t;

#define INDICATOR_MAGIC 0x8E 
#define EEPROM_INDICATOR_ADDR 2048

indicator_config_t g_ind_cfg;

// ========================== 3. 初始化 ==========================
void matrix_init_kb(void) {
    eeprom_read_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    
    if (g_ind_cfg.magic != INDICATOR_MAGIC) {
        for(uint8_t i=0; i<16; i++) {
            g_ind_cfg.layers[i] = (led_cfg_t){(uint8_t)(27-i), 1, 128, 255, 255};
        }
        g_ind_cfg.magic = INDICATOR_MAGIC;
        eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    }
    matrix_init_user();
}

void keyboard_post_init_user(void) {
    // 启用自定义静态模式
    rgb_matrix_mode(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
    jloled_init();
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

    return true;
}

// ========================== 6. RAW HID 指令集 ==========================
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    uint8_t magic = data[0];

    // --- 1. OLED 图像数据/命令处理 (0xAC, 0xB0-0xCF, 0xAD) ---
    // 检查是否是 OLED 相关的 Raw HID 命令
    if (magic == JLOLED_MAGIC_REALTIME || 
        (magic >= JLOLED_MAGIC_WRITE_EEPROM_BASE && magic < (JLOLED_MAGIC_WRITE_EEPROM_BASE + JLOLED_SLOT_COUNT)) ||
        magic == JLOLED_MAGIC_DISPLAY_SLOT) {
        
        jloled_receive(data, length);
        return; 
    }
    
    if (length < 2 || data[0] != 0xAB) return;
    
    switch (data[1]) {
    
        case 0x00: bootloader_jump(); break;
        case 0x01: eeconfig_init(); wait_ms(200); soft_reset_keyboard(); break;
        case 0x02: soft_reset_keyboard(); break;
        
        case 0x90: 
            g_remote_rgb_data.h = data[2]; g_remote_rgb_data.s = data[3]; 
            g_remote_rgb_data.v = data[4]; g_remote_rgb_data.spd = data[5]; 
            uint8_t copy_len = (length - 6 < MAX_LED_BYTE_COUNT) ? (length - 6) : MAX_LED_BYTE_COUNT;
            memcpy(g_remote_rgb_data.led_bitmap, &data[6], copy_len);
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
            break;
        
        case 0x91: rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_off); break;

        case 0x80: // Save
            g_ind_cfg.magic = INDICATOR_MAGIC;
            eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
            break;
        case 0x81: // Layers
            if (data[2] < 16) {
                g_ind_cfg.layers[data[2]].index = data[3];
                g_ind_cfg.layers[data[2]].count = data[4];
                g_ind_cfg.layers[data[2]].h     = data[5];
                g_ind_cfg.layers[data[2]].s     = data[6];
                g_ind_cfg.layers[data[2]].v     = data[7];
            }
            break;
        default: break;
    }
}
