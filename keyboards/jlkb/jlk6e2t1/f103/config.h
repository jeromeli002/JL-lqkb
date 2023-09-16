/* Copyright 2021 ZhaQian
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "config_common.h"

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x5009
#define DEVICE_VER      0x0001
#define MANUFACTURER    JL
#define PRODUCT         \u591a\u529f\u80fd\u952e\u76d8\u4f34\u4fa3

// #define ANALOG_JOYSTICK_X_AXIS_PIN A2     /* 上拉 X轴右移 */
// #define ANALOG_JOYSTICK_Y_AXIS_PIN A1     /* 上拉 Y轴下移 */
// #define ANALOG_JOYSTICK_CLICK_PIN B1

/* 摇杆映射*/ 
#ifdef JOYSTICK_TRIGGER_ENABLE
#define ADC_RESOLUTION ADC_CFGR1_RES_10BIT
#define JOYSTICK_ADC_RESOLUTION 10
#define JOYSTICK_USE_LPF
#define JOYSTICK_LPF_PROPORTION (0.2)
#define JOYSTICK_AXES_X_PIN { A1 }
#define JOYSTICK_AXES_Y_PIN { A2 }
#define JOYSTICK_AXES_PX_KEY_POS {3, 0}
#define JOYSTICK_AXES_NX_KEY_POS {3, 1}
#define JOYSTICK_AXES_PY_KEY_POS {4, 0}
#define JOYSTICK_AXES_NY_KEY_POS {4, 1}
#endif


/* 层数 */
#  define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 4

#define MATRIX_ROW_PINS {  C14, B8, B5, B0, B1 }
#define MATRIX_COL_PINS {  C13, C15, B4, A5}

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW


#define ENCODERS_PAD_A  { B10, B14, B12 }
#define ENCODERS_PAD_B  { B11, B15, B13 }
#define ENCODER_RESOLUTION 2
#define ENCODER_PAD_A_KEY_POS {3, 0}, {3, 2}, {0, 3}
#define ENCODER_PAD_B_KEY_POS {3, 1}, {3, 3}, {1, 3}

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


#define RGB_DI_PIN B3
#ifdef RGB_DI_PIN 
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 6
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif
