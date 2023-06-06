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
#define PRODUCT         \u8bbe\u8ba1\u5e08\u952e\u76d8

#define ANALOG_JOYSTICK_X_AXIS_PIN A1
#define ANALOG_JOYSTICK_Y_AXIS_PIN A2
#define ANALOG_JOYSTICK_CLICK_PIN B1

/* 层数 */
#  define DYNAMIC_KEYMAP_LAYER_COUNT 32

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 5

#define MATRIX_ROW_PINS {  C13, C14, C15, A3, A4  }
#define MATRIX_COL_PINS {  B9, B8, B5, B4, B14 }

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW


#define ENCODERS_PAD_A  { A5, A7, B3, A8 }
#define ENCODERS_PAD_B  { A6, B0, A15, B15 }
#define ENCODER_RESOLUTION 2
#define ENCODER_PAD_A_KEY_POS {4, 0}, {4, 2}, {4, 4}, {2, 4}
#define ENCODER_PAD_B_KEY_POS {4, 1}, {4, 3}, {2, 3}, {3, 4}

/* 编码器01 */
#define ENCODER_DEFAULT_POS 0x3

/* 指示灯 */
/* #define LED_CAPS_LOCK_PIN B10
#define LED_NUM_LOCK_PIN C13
#define LED_SCROLL_LOCK_PIN B1
#define LED_PIN_ON_STATE 0  指示灯 LED“亮”时指示灯引脚的状态 -1高电平，0低电平*/

/* number of backlight levels */

/*
#ifdef BACKLIGHT_PIN
#define BACKLIGHT_LEVELS 3
#endif
*/

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
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 9
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif
