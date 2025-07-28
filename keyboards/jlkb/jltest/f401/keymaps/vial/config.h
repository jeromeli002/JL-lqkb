#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 2 }

#define WS2812_POWER_PIN    A14
#define WS2812_BYTE_ORDER   WS2812_BYTE_ORDER_GRB

// 启用自定义矩阵灯驱动
#define CUSTOM_MATRIX_LIGHT_ENABLE

// 定义矩阵灯的行和列数量
#define MATRIX_LIGHT_ROWS    8
#define MATRIX_LIGHT_COLS    8

// 定义行引脚。请根据你的单片机和实际接线修改。
#define MATRIX_LIGHT_ROW_PINS { C13, C14, C15, C0, C1, C2, C3, A0 }

// 定义列引脚。请根据你的单片机和实际接线修改。
#define MATRIX_LIGHT_COL_PINS { A1, A2, A3, A4, A5, A6, A7, C4 }

// --- 新增镜像配置选项 ---
// 如果你的字符是左右反的，设置为 1。否则为 0。
#define MATRIX_LIGHT_FLIP_HORIZONTAL 1 // 默认为0，不进行左右镜像

// 如果你的字符是上下反的，设置为 1。否则为 0。
#define MATRIX_LIGHT_FLIP_VERTICAL   0 // 默认为0，不进行上下镜像

// 控制是否开启下雨效果 (1:开启, 0:关闭)
#define ENABLE_MATRIX_LIGHT_RAIN_EFFECT 1

// 无操作多久后显示下雨动画 (毫秒)
#define MATRIX_LIGHT_RAIN_TIMEOUT_MS    10000 // 例如，10秒无操作后开启