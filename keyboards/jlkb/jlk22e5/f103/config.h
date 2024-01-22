#pragma once
#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6004
#define DEVICE_VER      0x0002
#define MANUFACTURER    JL\u7684
#define PRODUCT         \u5927\u5e08\u4e13\u4e1a\u952e\u76d8

/*
#define ANALOG_JOYSTICK_X_AXIS_PIN A2   // 上拉 X轴右移
#define ANALOG_JOYSTICK_Y_AXIS_PIN A1   // 上拉 Y轴下移 
#define ANALOG_JOYSTICK_CLICK_PIN B1
#define POINTING_DEVICE_ROTATION_180   // 将 X 和 Y 数据旋转 180 度 
*/

#ifdef JOYSTICK_TRIGGER_ENABLE
#define ADC_RESOLUTION ADC_CFGR1_RES_10BIT
#define JOYSTICK_ADC_RESOLUTION 10
#define JOYSTICK_USE_LPF
#define JOYSTICK_LPF_PROPORTION (0.2)
#define JOYSTICK_AXES_X_PIN { A1 }
#define JOYSTICK_AXES_Y_PIN { A2 }
#define JOYSTICK_AXES_PX_KEY_POS {2, 4}
#define JOYSTICK_AXES_NX_KEY_POS {3, 4}
#define JOYSTICK_AXES_PY_KEY_POS {0, 4}
#define JOYSTICK_AXES_NY_KEY_POS {1, 4}
#endif

 /* 层指示灯 */
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK   
#define RGBLIGHT_MAX_LAYERS 16

/* 层数 */
#define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* key matrix size */
#define MATRIX_ROWS 7
#define MATRIX_COLS 7
 
 /* 层指示灯 
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK */

/* key matrix pins */
#define MATRIX_ROW_PINS { C13, C14, C15, A0, A3, A14, A13 }
#define MATRIX_COL_PINS { B9, B8, B5, B4, B3, A15, B13 }
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* 编码器 */
#define ENCODERS_PAD_A { A8, A4, A6, B0, B10, A10 }
#define ENCODERS_PAD_B { B15, A5, A7, B1, B11, B14 }
#define ENCODER_RESOLUTIONS {4, 4, 2, 4, 4, 4 }
#define ENCODER_PAD_A_KEY_POS {0, 5}, {2, 5}, {4, 5}, {6, 5}, {0, 6}, {2, 6}
#define ENCODER_PAD_B_KEY_POS {1, 5}, {3, 5}, {5, 5}, {4, 4}, {1, 6}, {3, 6}
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


#define RGB_DI_PIN B12
#ifdef RGB_DI_PIN 
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 22
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif