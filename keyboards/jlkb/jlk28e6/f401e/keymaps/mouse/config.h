#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 2 }

/*指点杆*/
#define ANALOG_JOYSTICK_X_AXIS_PIN A2     /* 上拉 X轴右移 */
#define ANALOG_JOYSTICK_Y_AXIS_PIN A1     /* 上拉 Y轴下移 */
//#define ANALOG_JOYSTICK_CLICK_PIN B1
#define POINTING_DEVICE_ROTATION_270   /* 将 X 和 Y 数据旋转 180 度 */
//#define POINTING_DEVICE_INVERT_X	（可选）反转 X 轴报告。
//#define POINTING_DEVICE_INVERT_Y	（可选）反转 Y 轴报告。

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