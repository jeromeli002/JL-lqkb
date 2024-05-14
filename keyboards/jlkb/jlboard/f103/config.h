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
#define PRODUCT         \u81ea\u5b9a\u4e49\u952e\u76d8

#define ANALOG_JOYSTICK_X_AXIS_PIN A1  
#define ANALOG_JOYSTICK_Y_AXIS_PIN A2  
// #define POINTING_DEVICE_ROTATION_180   /* 将 X 和 Y 数据旋转 180 度 */ 
#define POINTING_DEVICE_INVERT_X       /* 翻转X */ 
/* #define ANALOG_JOYSTICK_CLICK_PIN B1

#ifdef JOYSTICK_TRIGGER_ENABLE
#define ADC_RESOLUTION ADC_CFGR1_RES_10BIT
#define JOYSTICK_ADC_RESOLUTION 10
#define JOYSTICK_USE_LPF
#define JOYSTICK_LPF_PROPORTION (0.2)
#define JOYSTICK_AXES_X_PIN { A1 }
#define JOYSTICK_AXES_Y_PIN { A2 }
#define JOYSTICK_AXES_PX_KEY_POS {2, 5}
#define JOYSTICK_AXES_NX_KEY_POS {3, 5}
#define JOYSTICK_AXES_PY_KEY_POS {2, 6}
#define JOYSTICK_AXES_NY_KEY_POS {3, 6}
#endif
 */
 /* 层指示灯
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK   
#define RGBLIGHT_MAX_LAYERS 16
 */
 
/* 层数 */
#  define DYNAMIC_KEYMAP_LAYER_COUNT 8

/* key matrix size */
#define MATRIX_ROWS 7
#define MATRIX_COLS 18

#define MATRIX_ROW_PINS {  B14, B15, A8, A10, A13, A14 , 0}
#define MATRIX_COL_PINS {  C13, C14, C15, A15, B3, B4, B5, B8, B9, A0, A3, A4, A5, A6, A7, B0, B1, B10 }

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW
#define DEBOUNCE 5   //去抖时间5毫秒


#define ENCODERS_PAD_A  { B11, B6 }
#define ENCODERS_PAD_B  { B12, B7 }
#define ENCODER_RESOLUTION 4
#define ENCODER_PAD_A_KEY_POS {0, 7}, {3, 7}
#define ENCODER_PAD_B_KEY_POS {2, 7}, {5, 7}

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

/* 常规rgb灯*/
#define RGB_DI_PIN B13         /* rgb灯脚*/
#ifdef RGB_DI_PIN              
#define RGBLIGHT_ANIMATIONS    /* 启用全部灯效*/
#define RGBLED_NUM 110           /* rgb灯数量*/
#define RGBLIGHT_HUE_STEP 8    /* rgb灯循环色调步数*/
#define RGBLIGHT_SAT_STEP 8    /* rgb灯增加饱和度步数*/
#define RGBLIGHT_VAL_STEP 8    /* rgb灯增加亮度步数*/
#define RGBLIGHT_LIMIT_VAL 128 /* rgb灯最高亮度*/
#endif
