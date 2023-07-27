#pragma once
#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x5002
#define DEVICE_VER      0x0001
#define MANUFACTURER    JL
#define PRODUCT         \u591a\u529f\u80fd\u952e\u76d8-68

/* 层数 */
#define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* key matrix size */
#define MATRIX_ROWS 9
#define MATRIX_COLS 9

 
 /* 层指示灯 */
#define RGBLIGHT_LAYERS

/* key matrix pins */
#define MATRIX_ROW_PINS { B12, B13, B14, B15, A8, A10, A15, B3, A6 }
#define MATRIX_COL_PINS { B12, B13, B14, B15, A8, A10, A15, B3, A6 }
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

//* 编码器 */
#define ENCODERS_PAD_A { B0, A1 }
#define ENCODERS_PAD_B { A7, A0 }
#define ENCODER_RESOLUTIONS {2, 2}
#define ENCODER_PAD_A_KEY_POS {6, 0}, {6, 2}
#define ENCODER_PAD_B_KEY_POS {6, 1}, {6, 3}
/* 编码器01 */

/* 指示灯 */
#define LED_CAPS_LOCK_PIN B10
#define LED_NUM_LOCK_PIN C13
//#define LED_SCROLL_LOCK_PIN B1
#define LED_PIN_ON_STATE 0  /* 指示灯 LED“亮”时指示灯引脚的状态 -1高电平，0低电平*/

/* number of backlight levels */

/* #ifdef BACKLIGHT_PIN
#define BACKLIGHT_LEVELS 3
#endif   */

/* Set 0 if debouncing isn't needed */
/* #define DEBOUNCING_DELAY 5   */

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

#define RGB_DI_PIN B4
#ifdef RGB_DI_PIN
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 4
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif
/* OLED Configuration */
