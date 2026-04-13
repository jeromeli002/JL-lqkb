#pragma once

 /* 层指示灯 */
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK   
#define RGBLIGHT_MAX_LAYERS 16

/* 层数 */
#  define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* 指示灯 */
/* #define LED_CAPS_LOCK_PIN C8
#define LED_NUM_LOCK_PIN B15
#define LED_SCROLL_LOCK_PIN B13
#define LED_PIN_ON_STATE 1  // 指示灯 LED“亮”时指示灯引脚的状态 -1高电平，0低电平*/

/* number of backlight levels */



/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)


