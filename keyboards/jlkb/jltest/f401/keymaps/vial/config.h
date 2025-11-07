#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 2 }

// #define RAW_USAGE_PAGE   0xFF69
// #define RAW_USAGE_ID     0x66

#define WS2812_POWER_PIN    A14

// 启用自定义矩阵灯驱动
#define CUSTOM_MATRIX_LIGHT_ENABLE

// 定义矩阵灯的行和列数量
#define MATRIX_LIGHT_ROWS    8
#define MATRIX_LIGHT_COLS    8

// 定义行引脚。请根据你的单片机和实际接线修改。
#define MATRIX_LIGHT_ROW_PINS { C13, C14, C15, C0, C1, C2, C3, A0 }

// 定义列引脚。请根据你的单片机和实际接线修改。
#define MATRIX_LIGHT_COL_PINS { A1, A2, A3, A4, A5, A6, A7, C4 }

// 可选：覆盖默认配置（示例）
// #define MATRIX_LIGHT_DEFAULT_BRIGHTNESS  80    // 默认亮度100%
#define MATRIX_LIGHT_FLASH_COUNT  3          // 默认闪烁3次
// 可选配置：C2R 或 R2C（二选一，不配置则默认 C2R）
#define MATRIX_LIGHT_LEVEL_CONFIG  C2R  // 点亮时 COL=高电平，ROW=低电平（默认，符合需求）
// #define MATRIX_LIGHT_LEVEL_CONFIG  R2C  // 点亮时 COL=低电平，ROW=高电平（反向配置）