#pragma once
#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6007
#define DEVICE_VER      0x0002
#define MANUFACTURER    JL
#define PRODUCT         \u6447\u6746\u65cb\u94ae\u5c0f\u73a9\u610f\u513f

#define JOYSTICK_AXES_COUNT 4
#define JOYSTICK_BUTTON_COUNT 0

/* 层数 */
#define DYNAMIC_KEYMAP_LAYER_COUNT 4

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 5
 

/* key matrix pins */
#define MATRIX_ROW_PINS {  A6,B15,A8,C15,A3}
#define MATRIX_COL_PINS {  B10,B14,B8,B3,B9}

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

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

