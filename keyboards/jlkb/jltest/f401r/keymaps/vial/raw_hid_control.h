#ifndef RAW_HID_CONTROL_H
#define RAW_HID_CONTROL_H

#include "quantum.h"

// 声明存储 LED 数据的数组
// 外部控制模式下，每个 LED 的颜色都存储在这里
extern RGB external_led_colors[MATRIX_ROWS * MATRIX_COLS];

// 外部程序协议定义
#define EXTERNAL_CMD_MAIN 0xAB
#define EXTERNAL_CMD_SUB  0xB0
#define EXTERNAL_PACKET_SIZE 6 // 0xAB, 0xB0, R, G, B, LED_INDEX

// 声明 Raw HID 接收回调函数 (QMK 自动调用)
void raw_hid_receive(uint8_t *data, uint8_t length);

#endif // RAW_HID_CONTROL_H