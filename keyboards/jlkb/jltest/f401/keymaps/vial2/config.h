#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 2 }

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

// --- 新增镜像配置选项 ---
// 如果你的字符是左右反的，设置为 1。否则为 0。
#define MATRIX_LIGHT_FLIP_HORIZONTAL 0 // 默认为0，不进行左右镜像

// 如果你的字符是上下反的，设置为 1。否则为 0。
#define MATRIX_LIGHT_FLIP_VERTICAL   0 // 默认为0，不进行上下镜像

// 控制是否开启下雨效果 (1:开启, 0:关闭)
#define ENABLE_MATRIX_LIGHT_RAIN_EFFECT 1

// 无操作多久后显示下雨动画 (毫秒)
#define MATRIX_LIGHT_RAIN_TIMEOUT_MS    3000 // 例如，10秒无操作后开启

// 配置矩阵旋转角度
// #define MATRIX_LIGHT_ROTATION MATRIX_LIGHT_ROTATION_90   // 顺时针旋转90度
// #define MATRIX_LIGHT_ROTATION MATRIX_LIGHT_ROTATION_180  // 顺时针旋转180度
// #define MATRIX_LIGHT_ROTATION MATRIX_LIGHT_ROTATION_270  // 顺时针旋转270度
// 如果不定义 MATRIX_LIGHT_ROTATION，则默认不旋转 (MATRIX_LIGHT_ROTATION_NONE)