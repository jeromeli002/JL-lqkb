#include "quantum.h"
#include "jlrgb.h"
#include "tm1640.h"
#include "gpio.h"
#include "eeprom.h"
//#include "audio.c"
#include <string.h>

#define CLICK_HZ 500
#define CLICK_MS 2
#define CLICK_ENABLED 0

uint16_t click_hz = CLICK_HZ;
uint16_t click_time = CLICK_MS;
uint8_t click_toggle = CLICK_ENABLED;
void clicking_notes(uint16_t freq, uint16_t duration) {
#ifdef AUDIO_ENABLE
    if (freq >= 100 && freq <= 20000 && duration < 100) {
        play_note(freq, 10);
        for (uint16_t i = 0; i < duration; i++) {
            wait_ms(1);
        }
        stop_all_notes();
    }
#endif
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    #ifdef AUDIO_ENABLE
    if (click_toggle && record->event.pressed) {
        clicking_notes(click_hz, click_time);
    }
#endif
    return process_record_user(keycode, record);
}

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
    uint8_t magic; 
} indicator_config_t;

#define INDICATOR_MAGIC 0x8E 
#define EEPROM_INDICATOR_ADDR 4096

indicator_config_t g_ind_cfg;

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
        g_ind_cfg.num  = (led_cfg_t){1, 1, 85, 10, 10};
        g_ind_cfg.scrl = (led_cfg_t){2, 1, 170, 255, 255};
        for(uint8_t i=0; i<16; i++) {
            g_ind_cfg.layers[i] = (led_cfg_t){(uint8_t)(27-i), 1, 10, 10, 10};
        }
        g_ind_cfg.magic = INDICATOR_MAGIC;
        eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    }
    matrix_init_user();
}

// ========================== 4. 初始化 ==========================
void keyboard_post_init_user(void) {
    // 启用音频播放
    #ifdef AUDIO_ENABLE
        clicking_notes(880, 50);
    #endif
    // 启用自定义静态模式
    rgb_matrix_mode(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
}

// ========================== 4. 主循环 (仅保留 TM1640) ==========================
void matrix_scan_kb(void) {
    tm1640_task(); 
    matrix_scan_user();
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
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (length < 2 || data[0] != 0xAB) return;

    switch (data[1]) {
    
        case 0x00: bootloader_jump(); break;
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

        case 0x82: // Caps
            g_ind_cfg.caps.index = data[3]; g_ind_cfg.caps.count = data[4];
            g_ind_cfg.caps.h = data[5]; g_ind_cfg.caps.s = data[6]; g_ind_cfg.caps.v = data[7];
            break;
        case 0x83: // Num
            g_ind_cfg.num.index = data[3]; g_ind_cfg.num.count = data[4];
            g_ind_cfg.num.h = data[5]; g_ind_cfg.num.s = data[6]; g_ind_cfg.num.v = data[7];
            break;
        case 0x84: // Scrl
            g_ind_cfg.scrl.index = data[3]; g_ind_cfg.scrl.count = data[4];
            g_ind_cfg.scrl.h = data[5]; g_ind_cfg.scrl.s = data[6]; g_ind_cfg.scrl.v = data[7];
            break;
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