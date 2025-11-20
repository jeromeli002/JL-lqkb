#pragma once

#include "quantum.h"

// 定义OLED屏幕尺寸，根据实际情况修改
// 常见的有 128x32 (512字节) 或 128x64 (1024字节)
#ifndef OLED_DISPLAY_WIDTH
#   define OLED_DISPLAY_WIDTH 128
#endif
#ifndef OLED_DISPLAY_HEIGHT
#   define OLED_DISPLAY_HEIGHT 32
#endif

// 计算缓冲区大小
#define JLOLED_BUFFER_SIZE (OLED_DISPLAY_WIDTH * OLED_DISPLAY_HEIGHT / 8)

// 每次Raw HID传输的数据包大小 (QMK默认为32)
#define RAWHID_PACKET_SIZE 32
// 每次传输的有效数据负载 (扣除 0xAC 和 索引字节)
#define PAYLOAD_SIZE (RAWHID_PACKET_SIZE - 2)

// 函数声明
void jloled_init(void);
void jloled_task(void);
void jloled_receive(uint8_t *data, uint8_t length);