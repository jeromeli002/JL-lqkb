#pragma once
#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6007
#define DEVICE_VER      0x0002
#define MANUFACTURER    \u722c\u4e0a\u5899\u5934\u62fd\u7ea2\u674f
#define PRODUCT         \u5c0f\u73a9\u5177\u952e\u76d8

/* 层数 */
#define DYNAMIC_KEYMAP_LAYER_COUNT 19

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 5
 
 /* 层指示灯 */
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK

/* key matrix pins */
#define MATRIX_ROW_PINS {  A6,B15,A8,A1,A3}
#define MATRIX_COL_PINS {  B10,B14,B8,B3,B9}

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* 编码器 */
#define ENCODERS_PAD_A { A7, A15, B4, A5 }
#define ENCODERS_PAD_B { B0, A10, B5, A4 }
#define ENCODER_RESOLUTIONS  { 2, 2, 2, 2 }
#define ENCODER_PAD_A_KEY_POS {0, 3}, {2, 3}, {0,4}, {2, 4}
#define ENCODER_PAD_B_KEY_POS {1, 3}, {3, 3}, {1,4}, {3, 4}

/* 编码器01 */
#define ENCODER_DEFAULT_POS 0x3

/* 指示灯 */
/* #define LED_CAPS_LOCK_PIN B10
#define LED_NUM_LOCK_PIN C13
#define LED_SCROLL_LOCK_PIN B1
#define LED_PIN_ON_STATE 0  指示灯 LED“亮”时指示灯引脚的状态 -1高电平，0低电平*/

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


#define RGB_DI_PIN B1
#ifdef RGB_DI_PIN 
#define RGBLED_NUM 7
#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif