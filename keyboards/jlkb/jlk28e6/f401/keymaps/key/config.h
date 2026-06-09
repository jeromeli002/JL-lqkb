#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 2 }

#define RAW_USAGE_PAGE   0xFF69
// #define RAW_USAGE_ID     0x66

// 矩阵位置
#define JOYSTICK_ACTUATION 256  // 触发点，即摇杆偏移多少开始触发按键 (0-511)
#define JOYSTICK_HYSTERESIS 50  // 滞后量，防止抖动，调整此值以达到最佳效果
#define JOYSTICK_KEY_X_PIN  {A1}
#define JOYSTICK_KEY_Y_PIN  {A2}
#define JOYSTICK_KEY_PX_POS {{1, 6}}  //高电平
#define JOYSTICK_KEY_NX_POS {{0, 6}}  //低电平
#define JOYSTICK_KEY_PY_POS {{2, 6}}  //高电平
#define JOYSTICK_KEY_NY_POS {{3, 6}}  //低电平

// 摇杆按键数 Max 32
#define JOYSTICK_BUTTON_COUNT 0
// 每个摇杆轴数量 Max 6: X, Y, Z, Rx, Ry, Rz
#define JOYSTICK_AXIS_COUNT 2 //轴数量X/Y轴

//  OLED设置项
// #define OLED_FONT_H "glcdfont.c"//自定义字体
// #define OLED_FONT_WIDTH 6 //字体宽度
// #define OLED_FONT_HEIGHT 8 //字体高度
// #define OLED_FADE_OUT         //动画淡出
// #define OLED_FADE_OUT_INTERVAL 5     //淡出速度0~15 越大越快
#define OLED_SCROLL_TIMEOUT	 5000 //2000毫秒后开始滚动
#define OLED_SCROLL_TIMEOUT_RIGHT //向右滚动 不设置默认向左滚动
#define OLED_TIMEOUT 0  // 10分钟（毫秒）无操作后关闭屏幕

// [新增] 覆盖默认的 200ms 超时，例如设置为 10s
#define JLOLED_REALTIME_TIMEOUT 2000