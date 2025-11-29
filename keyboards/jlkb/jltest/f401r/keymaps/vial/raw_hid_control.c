#include "raw_hid_control.h"
#include "rgb_matrix.h"

// 实际存储 LED 颜色的数组
// 注意: 使用 RGB_MATRIX_LED_COUNT 来确保数组大小正确
RGB external_led_colors[RGB_MATRIX_LED_COUNT] = {0}; 

// QMK Raw HID 接收回调函数
void raw_hid_receive(uint8_t *data, uint8_t length) {
    
    // 1. 检查数据包长度是否正确
    if (length < EXTERNAL_PACKET_SIZE) {
        return;
    }
    
    // 2. 检查主命令和子命令是否匹配
    if (data[0] != EXTERNAL_CMD_MAIN || data[1] != EXTERNAL_CMD_SUB) {
        return;
    }

    // 协议解析: data[0]=0xAB, data[1]=0xB0
    uint8_t r         = data[2]; // R 
    uint8_t g         = data[3]; // G
    uint8_t b         = data[4]; // B
    uint8_t led_index = data[5]; // LED 位置

    // 3. 检查 LED 索引是否在合法范围内
    if (led_index < RGB_MATRIX_LED_COUNT) {
        // 4. 更新存储的颜色
        external_led_colors[led_index].r = r;
        external_led_colors[led_index].g = g;
        external_led_colors[led_index].b = b;
    }
    
    // 如果启用了 RGB Matrix，并且当前处于外部控制模式，需要立即刷新
    if (rgb_matrix_is_enabled()) {
        rgb_matrix_update_flags(RGB_MATRIX_FLAG_OVERRIDE_MODE);
    }
}