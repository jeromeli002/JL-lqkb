#pragma once

#include "quantum.h"

// 宏定义：LED 位图占用的字节数 (从第 7 字节到第 32 字节，共 26 字节)
#define MAX_LED_BYTE_COUNT 26 
// 实际支持的最大 LED 数量
#define MAX_SUPPORTED_LEDS (MAX_LED_BYTE_COUNT * 8) // 208 个 LED

// 声明全局静态变量，初始默认值均为 55
// 这是一个结构体，用于在不同文件间传递数据
typedef struct {
    uint8_t h; // Hue, 对应接收数据的第 3 字节 (索引 2)
    uint8_t s; // Saturation, 对应接收数据的第 4 字节 (索引 3)
    uint8_t v; // Value/Brightness, 对应接收数据的第 5 字节 (索引 4)
    uint8_t spd; // Speed, 对应接收数据的第 6 字节 (索引 5)
    uint8_t led_bitmap[MAX_LED_BYTE_COUNT];  // 新增：LED 选中位图数据，从第 7 字节 (索引 6) 开始
} remote_rgb_data_t;

// 声明一个外部可访问的全局变量
extern remote_rgb_data_t g_remote_rgb_data;

// 声明数据设置函数，此函数将在接收到数据时调用
void set_remote_rgb(const uint8_t *data, uint8_t len);