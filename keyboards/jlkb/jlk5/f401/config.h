#pragma once

 /* 层指示灯
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK   
#define RGBLIGHT_MAX_LAYERS 4
 */
 
#define RAW_USAGE_PAGE   0xFF69
#define RAW_USAGE_ID     0x66

/* 层数 */
#  define DYNAMIC_KEYMAP_LAYER_COUNT 8

// QMK宏数量，最多128个
#define DYNAMIC_KEYMAP_MACRO_COUNT 128
#define DYNAMIC_MACRO_SIZE	256

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

