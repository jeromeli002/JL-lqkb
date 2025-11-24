#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 2 }

// #define RAW_USAGE_PAGE   0xFF69
// #define RAW_USAGE_ID     0x66

#define OLED_DISPLAY_128X64  //128x64分辨率

//  OLED设置项
//#define OLED_FONT_H "glcdfont.c"//自定义字体
//#define OLED_FONT_WIDTH 6 //字体宽度
//#define OLED_FONT_HEIGHT 8 //字体高度
#define OLED_FADE_OUT         //动画淡出
#define OLED_FADE_OUT_INTERVAL 5     //淡出速度0~15 越大越快
// #define OLED_SCROLL_TIMEOUT	 5000 //2000毫秒后开始滚动
// #define OLED_SCROLL_TIMEOUT_RIGHT //向右滚动 不设置默认向左滚动
#define OLED_TIMEOUT 10000  // 10分钟（毫秒）无操作后关闭屏幕