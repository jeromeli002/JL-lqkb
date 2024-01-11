#pragma once
#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6004
#define DEVICE_VER      0x0002
#define MANUFACTURER    JL\u7684
#define PRODUCT         \u672a\u6210\u5e74\u952e\u76d8-42

//#define ANALOG_JOYSTICK_X_AXIS_PIN A2   /* 上拉 X轴右移 */
//#define ANALOG_JOYSTICK_Y_AXIS_PIN A1   /* 上拉 Y轴下移 */
//#define ANALOG_JOYSTICK_CLICK_PIN B1
//#define POINTING_DEVICE_ROTATION_180   /* 将 X 和 Y 数据旋转 180 度 */

#ifdef JOYSTICK_TRIGGER_ENABLE
#define ADC_RESOLUTION ADC_CFGR1_RES_10BIT
#define JOYSTICK_ADC_RESOLUTION 10
#define JOYSTICK_USE_LPF
#define JOYSTICK_LPF_PROPORTION (0.2)
#define JOYSTICK_AXES_X_PIN { A2 }
#define JOYSTICK_AXES_Y_PIN { A1 }
#define JOYSTICK_AXES_PX_KEY_POS {10, 4}
#define JOYSTICK_AXES_NX_KEY_POS {11, 4}
#define JOYSTICK_AXES_PY_KEY_POS {8, 4}
#define JOYSTICK_AXES_NY_KEY_POS {9, 4}
#endif

 /* 层指示灯 */
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK   
#define RGBLIGHT_MAX_LAYERS 16

/* 层数 */
#define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 12
 
 /* 层指示灯 
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK */

/* key matrix pins */
#define MATRIX_ROW_PINS { A5, A6, B1, A0, B11 }
#define MATRIX_COL_PINS { C14, C13, B5, B4, B3, B0, A7, A4, A3, A13, C15, B10 }
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* 编码器 */
#define ENCODERS_PAD_A { A15, A8  }
#define ENCODERS_PAD_B { A14, B15 }
#define ENCODER_RESOLUTIONS {2, 2, }
#define ENCODER_PAD_A_KEY_POS {0, 4}, {2, 4}
#define ENCODER_PAD_B_KEY_POS {1, 4}, {3, 4}
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


#define RGB_DI_PIN B9
#ifdef RGB_DI_PIN 
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 42
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif