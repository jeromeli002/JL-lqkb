#pragma once
#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6004
#define DEVICE_VER      0x0002
#define MANUFACTURER    JL
#define PRODUCT         \u9707\u52a8\u952e\u76d8

 /* 层指示灯 */
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK   
#define RGBLIGHT_MAX_LAYERS 16

/* 层数 */
#define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* key matrix size */
#define MATRIX_ROWS 3
#define MATRIX_COLS 6
 

/* key matrix pins */
#define MATRIX_ROW_PINS {  A15,B3,B12}
#define MATRIX_COL_PINS {  B11,B10,B1,B0,A7,A6}
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* 编码器 */
#define ENCODERS_PAD_A { B4, A13  }
#define ENCODERS_PAD_B { B5, A14  }
#define ENCODER_RESOLUTIONS {2, 2 }
#define ENCODER_PAD_A_KEY_POS {0, 2}, {3, 2}
#define ENCODER_PAD_B_KEY_POS {2, 2}, {5, 2}
/* 编码器01 */
#define ENCODER_DEFAULT_POS 0x3

/* 指示灯 */
// #define LED_CAPS_LOCK_PIN B10
#define LED_NUM_LOCK_PIN C13
// #define LED_SCROLL_LOCK_PIN B1
#define LED_PIN_ON_STATE 1  //指示灯 LED“亮”时指示灯引脚的状态 1高电平，0低电平

/* number of backlight levels */

#ifdef BACKLIGHT_PIN
#define BACKLIGHT_LEVELS 3
#endif

/* Set 0 if debouncing isn't needed */
/* #define DEBOUNCING_DELAY 5 */

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* prevent stuck modifiers */
/* #define PREVENT_STUCK_MODIFIERS */

/* 常规rgb灯*/
#define RGB_DI_PIN A1         /* rgb灯脚*/
#ifdef RGB_DI_PIN              
#define RGBLIGHT_ANIMATIONS    /* 启用全部灯效*/
#define RGBLED_NUM 8           /* rgb灯数量*/
#define RGBLIGHT_HUE_STEP 8    /* rgb灯循环色调步数*/
#define RGBLIGHT_SAT_STEP 8    /* rgb灯增加饱和度步数*/
#define RGBLIGHT_VAL_STEP 8    /* rgb灯增加亮度步数*/
#define RGBLIGHT_LIMIT_VAL 128 /* rgb灯最高亮度*/
#endif
