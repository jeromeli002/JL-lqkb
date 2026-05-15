#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 1, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 1, 5 }


#define WEAR_LEVELING_LOGICAL_SIZE 1024*16
#define WEAR_LEVELING_BACKING_SIZE WEAR_LEVELING_LOGICAL_SIZE*2

//#define BACKLIGHT_PINS { B0, B10, B0 }
//#define BACKLIGHT_LEVELS 31      // 设置亮度的总阶数（例如 0-31 级）
//#define BACKLIGHT_DEFAULT_LEVEL 1 // 设置开机时的默认亮度等级
//#define BACKLIGHT_LIMIT_VAL 28 // 设置最大亮度为 50% (范围 0-255)

/* 指示灯 */
//#define LED_CAPS_LOCK_PIN   B1
//#define LED_NUM_LOCK_PIN    B10
//#define LED_SCROLL_LOCK_PIN B0
//#define LED_PIN_ON_STATE 1  // 指示灯 LED“亮”时指示灯引脚的状态 1高电平，0低电平

