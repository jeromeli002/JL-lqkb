#include "quantum.h"
#include "jlrgb.h"
#include "tm1640.h"
#include "gpio.h"
#include <string.h>

// TM1640------------------- TM1640 --------------------------
// -------------------------- 宏定义 --------------------------
#define MATRIX_LIGHT_ROWS TM1640_ROWS
#define MATRIX_LIGHT_COLS TM1640_COLS

// -------------------------- 函数声明 --------------------------
void handle_external_bitmap_data(const uint8_t *data, uint16_t length, tm1640_brightness_t brightness);

// RGB------------------- RGB --------------------------
// 【已修改】：默认值 V (亮度) 设为 255 (0xFF)，确保变量默认值不会是熄灭状态。
remote_rgb_data_t g_remote_rgb_data = {
    .h = 79,
    .s = 255,
    .v = 25,
    .spd = 255
};
// RGB------------------- RGB --------------------------

// -------------------------- QMK 初始化流程 --------------------------

// 第一步：硬件初始化 (上电最先执行)
void matrix_init_kb(void) {
    tm1640_init();
    tm1640_start_blink(); // TM1640 默认效果
    matrix_init_user();
}

void keyboard_post_init_user(void) {
  rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_mode0); // rgb默认模式0
}

// 在主循环中调用非阻塞式任务
void matrix_scan_kb(void) {
    tm1640_task(); 
    matrix_scan_user();
}
// TM1640------------------- TM1640 --------------------------


// -------------------------- HID 接收回调函数 --------------------------
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    // 帧头检查
    if (data[0] != 0xAB ) return;

    switch (data[1]) {
        case 0xA0: // 关闭所有灯光
            tm1640_display_off();
            break;
            
        case 0xA1: // 精准点阵显示
            handle_external_bitmap_data(data, length, TM1640_DEFAULT_BRIGHTNESS_CMD);
            break;

        case 0xA2: // 重启并进入 Bootloader 模式
            bootloader_jump();
            break;

        case 0xA3: // 清空 EEPROM 配置并重启
            eeconfig_init();
            wait_ms(200); 
            soft_reset_keyboard();
            break;

        case 0xA4: // 正常重启
            soft_reset_keyboard();
            break;
            
        case 0xA5: // 流水灯 
            tm1640_start_running_light(); 
            break;

        case 0xA6: // 闪烁效果
            tm1640_start_blink();
            break;

        case 0xA7: // 关闭C1 (设置为输入)
            setPinInput(C1);
            break;
            
        case 0xA8: // C1 引脚置低
            setPinOutput(C1);
            writePinLow(C1);
            break;
            
         case 0xA9: // 关闭B5 (设置为输入)
            setPinInput(B5);
            break;
            
        case 0xAA: // B5 引脚置低
            setPinOutput(B5);
            writePinLow(B5);
            break;
            
        case 0xB0: // B5 rgb灯控制
            g_remote_rgb_data.h = data[2]; 
            g_remote_rgb_data.s = data[3]; 
            g_remote_rgb_data.v = data[4]; 
            g_remote_rgb_data.spd = data[5]; 
        if (length >= 6 + MAX_LED_BYTE_COUNT) {
                memcpy(g_remote_rgb_data.led_bitmap, &data[6], MAX_LED_BYTE_COUNT);
            }
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
            break;
            
       case 0xB1: // B5 关闭所有rgb灯
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_off);
            break;
            
      case 0xB2: // B5 rgb灯控制
            g_remote_rgb_data.h = 214; 
            g_remote_rgb_data.s = 0xFF; 
            g_remote_rgb_data.v = 0xFF; 
            g_remote_rgb_data.spd = 0xFF; 
            g_remote_rgb_data.led_bitmap[0] = 0xA0;
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
            break;

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