#pragma once
#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x5001
#define DEVICE_VER      0x0001
#define MANUFACTURER    JL
#define PRODUCT         \u8bbe\u8ba1\u5e08\u952e\u76d8

//#define ANALOG_JOYSTICK_X_AXIS_PIN A1   /* 上拉 X轴右移 */
//#define ANALOG_JOYSTICK_Y_AXIS_PIN A2   /* 上拉 Y轴下移 */
// #define ANALOG_JOYSTICK_CLICK_PIN B1



 /* 层指示灯 */
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK   
#define RGBLIGHT_MAX_LAYERS 16

/* 层数 */
#  define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 7
 
 /* 层指示灯
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK
*/
 
/* key matrix pins */
#define MATRIX_ROW_PINS { B5,B6,B7,B8,B9,A15 }
#define MATRIX_COL_PINS { B12,B13,B14,B15,A8,A10,B3 }

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* 编码器 */
#define ENCODERS_PAD_A { B0, A6 , A4, A3 }
#define ENCODERS_PAD_B { A7, A5 , A2, C13 }
#define ENCODER_RESOLUTIONS { 4,4,4,4 }
/* 映射矩阵 C，R*/
#define ENCODER_PAD_A_KEY_POS {1, 1}, {6, 2}, {6, 3}, {4, 3}
#define ENCODER_PAD_B_KEY_POS {0, 0}, {5, 2}, {5, 3}, {5, 3}

/* 编码器01 */
#define ENCODER_DEFAULT_POS 0x3

/* 指示灯 */
/* #define LED_CAPS_LOCK_PIN B10
#define LED_NUM_LOCK_PIN C13
#define LED_SCROLL_LOCK_PIN B1
#define LED_PIN_ON_STATE 0  指示灯 LED“亮”时指示灯引脚的状态 -1高电平，0低电平*/

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


#define RGB_DI_PIN B4
#ifdef RGB_DI_PIN 
#define RGBLED_NUM 16
#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif