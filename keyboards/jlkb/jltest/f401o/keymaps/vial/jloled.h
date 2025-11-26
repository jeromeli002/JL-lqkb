#pragma once

#include "quantum.h"

// --- 配置参数 (请在 config.h 中定义) ---
#ifndef OLED_DISPLAY_WIDTH
    #define OLED_DISPLAY_WIDTH 128
#endif

#ifndef OLED_DISPLAY_HEIGHT
    #define OLED_DISPLAY_HEIGHT 32
#endif

// 计算 RAM 缓冲区大小：依赖于 OLED 尺寸
// 默认 (128x32) -> 512 字节
#define JLOLED_BUFFER_SIZE (OLED_DISPLAY_WIDTH * OLED_DISPLAY_HEIGHT / 8)

// 图像库配置
#ifndef JLOLED_SLOT_COUNT
    // EEPROM 中可存储的图像槽位数量
    #define JLOLED_SLOT_COUNT 32
#endif

// **确保默认值的关键部分：**
#ifndef JLOLED_SLOT_SIZE 
    // 如果未定义，则默认与显示缓冲区大小一致 (128x32 默认为 512 字节)
    #define JLOLED_SLOT_SIZE JLOLED_BUFFER_SIZE 
#endif 

#ifndef JLOLED_EEPROM_SIZE
    // 整个图像库所需的 EEPROM 空间
    #define JLOLED_EEPROM_SIZE (JLOLED_SLOT_COUNT * JLOLED_SLOT_SIZE)
#endif

// EEPROM 存储起始地址偏移量 (仅声明为变量)
extern uint16_t jloled_eeprom_start_addr; 

// Raw HID 传输定义
#define RAWHID_PACKET_SIZE 32
#define PAYLOAD_SIZE (RAWHID_PACKET_SIZE - 2) // 30 字节有效负载 (减去 魔数 和 索引)

// 函数声明
void jloled_init(void);
void jloled_task(void);
void jloled_receive(uint8_t *data, uint8_t length);
void jloled_display_slot(uint8_t slot_index);

// Raw HID 魔数定义
#define JLOLED_MAGIC_REALTIME 0xAC           // 实时显示图像 (写入 RAM 缓冲区)
#define JLOLED_MAGIC_WRITE_EEPROM_BASE 0xB0  // 写入 EEPROM 槽位的基址 (0xB0 - 0xCF)
#define JLOLED_MAGIC_DISPLAY_SLOT 0xAD       // 调用 EEPROM 槽位显示