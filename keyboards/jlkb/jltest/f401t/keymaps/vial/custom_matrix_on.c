#include "quantum.h"
#include "tm1640.h"
#include "gpio.h"
#include <string.h>

// -------------------------- 宏定义 --------------------------
#define MATRIX_LIGHT_ROWS TM1640_ROWS
#define MATRIX_LIGHT_COLS TM1640_COLS

// -------------------------- 函数声明 --------------------------
void handle_external_bitmap_data(const uint8_t *data, uint16_t length, tm1640_brightness_t brightness);

// -------------------------- QMK 初始化和任务函数 --------------------------
void matrix_init_kb(void) {
    tm1640_init();
    tm1640_start_blink(); // 默认效果
    matrix_init_user();
}

// 在主循环中调用非阻塞式任务
void matrix_scan_kb(void) {
    tm1640_task(); 
    matrix_scan_user();
}


// -------------------------- HID 接收回调函数 --------------------------
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    // 帧头检查
    if (data[0] != 0xAB ) return;

    switch (data[1]) {
        
        case 0x00: clear_keyboard();bootloader_jump(); break;
        case 0x01: eeconfig_init(); wait_ms(200); soft_reset_keyboard(); break;
        case 0x02: soft_reset_keyboard(); break;
        
        case 0x10: tm1640_display_off(); break;
        case 0x11: handle_external_bitmap_data(data, length, TM1640_DEFAULT_BRIGHTNESS_CMD); break;
        case 0x12: tm1640_start_running_light(); break;
        case 0x13: tm1640_start_blink(); break;

        case 0x20: setPinInput(C1); break;    
        case 0x21: setPinOutput(C1); writePinLow(C1);  break;    
        case 0x22: setPinInput(B5); break;    
        case 0x23:  setPinOutput(B5); writePinLow(B5); break;

        default:
            break;
    }
}

// -------------------------- 外部数据处理函数实现 --------------------------
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
            
            const uint8_t byte_offset = col / 8;
            const uint8_t bit_idx = col % 8;
            
            if (current_row_data[byte_offset] & (1 << bit_idx)) {
                tm1640_col_data[col] |= (1 << row);
            }
        }
    }

    tm1640_display_bitmap(tm1640_col_data, brightness);
}