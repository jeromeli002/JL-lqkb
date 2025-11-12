#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 2 }

// #define RAW_USAGE_PAGE   0xFF69
// #define RAW_USAGE_ID     0x66

#define TM1640_SCLK_PIN B6
#define TM1640_DIN_PIN  B7

// 自定义行列数（覆盖默认8行×8列）
#define TM1640_ROWS 8
#define TM1640_COLS 8

// 自定义闪烁参数（覆盖默认值）
#define TM1640_BLINK_COUNT 3    // 闪烁5次
#define TM1640_BLINK_INTERVAL 200 // 闪烁间隔300ms

// 自定义流水速度（覆盖默认150ms/颗）
#define TM1640_RUNNING_SPEED 1000 // 流水速度100ms/颗（更快）

// 【新增配置】流水灯方向
// 0: 纵向 (默认, 先列后行)
// 1: 横向 (先行后列)
#define TM1640_RUNNING_DIRECTION 1

// 默认亮度7 (0-7 映射到 0x88-0x8F)
#define TM1640_DEFAULT_BRIGHTNESS 0 // 最大亮度