#pragma once
#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6666
#define DEVICE_VER      0x0002
#define MANUFACTURER    JL_dial
#define PRODUCT         JL_dial

/* 层数 */
#define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS 4
 
 /* 层指示灯 
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK */

/* key matrix pins */
#define MATRIX_ROW_PINS {  A6,C14,C15,A2}
#define MATRIX_COL_PINS {  A0,B1,A8,A1}

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* 编码器 */
#define ENCODERS_PAD_A  {B12}
#define ENCODERS_PAD_B  {B10}
#define ENCODER_RESOLUTION 2
#define ENCODER_PAD_A_KEY_POS {0, 2}/* 逆向 Col Row*/
#define ENCODER_PAD_B_KEY_POS {2, 2}/* 正向 Col Row*/
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


#define RGB_DI_PIN A13
#ifdef RGB_DI_PIN 
#define RGBLED_NUM 8
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif