#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 2 }

// 定义一个用户数据的基准地址，跳过前 1200 字节（通常足够 VIA 存 4-8 层键位）
// 具体的偏移量取决于你的键盘有多少层
#define USER_DATA_BASE_ADDR 4096 

// 指示灯配置紧跟基准地址
#define EEPROM_INDICATOR_ADDR (USER_DATA_BASE_ADDR)

// OLED 槽位紧跟指示灯配置之后
// 计算指示灯结构体的大小，确保 OLED 存储不会覆盖它
#define EEPROM_OLED_START_ADDR (EEPROM_INDICATOR_ADDR + 100)

#define WEAR_LEVELING_LOGICAL_SIZE 1024*24
#define WEAR_LEVELING_BACKING_SIZE WEAR_LEVELING_LOGICAL_SIZE*2
//  OLED设置项
#define OLED_DISPLAY_128X32  //128x64分辨率
// #define OLED_ROTATION OLED_ROTATION_180  //旋转角度
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