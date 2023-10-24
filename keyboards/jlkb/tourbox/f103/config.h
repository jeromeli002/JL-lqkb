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
/*
#define ANALOG_JOYSTICK_X_AXIS_PIN A1  
#define ANALOG_JOYSTICK_Y_AXIS_PIN A2   
#define ANALOG_JOYSTICK_CLICK_PIN B1
*/
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

 /* 层指示灯 */
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_LAYER_BLINK   
#define RGBLIGHT_MAX_LAYERS 16

/* 层数 */
#  define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* key matrix size */
#define MATRIX_ROWS 9
#define MATRIX_COLS 4

#define MATRIX_ROW_PINS {  B1, A7, A5, A3, C14, B10, B11, B12, B13  }
#define MATRIX_COL_PINS {  B0, A6, A4, C15 }

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW
#define DEBOUNCE 5   //去抖时间5毫秒


#define ENCODERS_PAD_A  { B4, B9, B3, A8 }
#define ENCODERS_PAD_B  { B5, B8, A15, B15 }
#define ENCODER_RESOLUTION 2
#define ENCODER_PAD_A_KEY_POS {0, 5}, {0, 6}, {0, 7}, {0, 8}
#define ENCODER_PAD_B_KEY_POS {1, 5}, {1, 6}, {1, 7}, {1, 8}

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
#define RGB_DI_PIN C13         /* rgb灯脚*/
#ifdef RGB_DI_PIN              
#define RGBLIGHT_ANIMATIONS    /* 启用全部灯效*/
#define RGBLED_NUM 9           /* rgb灯数量*/
#define RGBLIGHT_HUE_STEP 8    /* rgb灯循环色调步数*/
#define RGBLIGHT_SAT_STEP 8    /* rgb灯增加饱和度步数*/
#define RGBLIGHT_VAL_STEP 8    /* rgb灯增加亮度步数*/
#define RGBLIGHT_LIMIT_VAL 128 /* rgb灯最高亮度*/
#endif

/* RGN Matrix 
#ifdef RGB_MATRIX_ENABLE
#    define RGB_DI_PIN C13
#    define RGBLED_NUM 9
#    define DRIVER_LED_TOTAL RGBLED_NUM

#    define WS2812_PWM_DRIVER PWMD3               // default: PWMD2
#    define WS2812_PWM_CHANNEL 2                  // default: 2
#    define WS2812_PWM_PAL_MODE 2                 // Pin "alternate function", see the respective datasheet for the appropriate values for your MCU. default: 2
#    define WS2812_DMA_STREAM STM32_DMA1_STREAM3  // DMA Stream for TIMx_UP, see the respective reference manual for the appropriate values for your MCU.
#    define WS2812_DMA_CHANNEL 3                  // DMA Channel for TIMx_UP, see the respective reference manual for the appropriate values for your MCU.

#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200     // 最大亮度限制
#    define RGB_DISABLE_WHEN_USB_SUSPENDED true
#    define RGB_MATRIX_CENTER { 96, 32 }
#    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#endif
*/

/* OLED */
/* 
#ifdef OLED_DRIVER_ENABLE
#    define OLED_DISPLAY_128X64
#    define I2C_DRIVER I2CD1
//#    define OLED_IC OLED_IC_SH1106  // 选用1.3寸SH1106主控OLED时打开, 0.96寸SSD1306主控OLED注释掉本行。
//#    define OLED_COLUMN_OFFSET 2  // 调整OLED纵向偏移，0.96寸SSD1306主控OLED注释掉本行。
#    define I2C1_TIMINGR_PRESC 0U
#    define I2C1_TIMINGR_SCLDEL 7U
#    define I2C1_TIMINGR_SDADEL 0U
#    define I2C1_TIMINGR_SCLH 38U
#    define I2C1_TIMINGR_SCLL 129U
#    define OLED_TIMEOUT 60000  // 60秒无操作关闭OLED
#    define OLED_FONT_HEIGHT 8          //字体高度
#    define OLED_FONT_WIDTH 6           //字体宽度
#    define OLED_FONT_H "neil_font.c"   //字体
#endif
 */
 
/* 矩阵灯
#ifdef RGB_MATRIX_ENABLE
    #define DRIVER_LED_TOTAL 9
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200
    #define RGB_MATRIX_STARTUP_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
    #define RGB_MATRIX_KEYPRESSES
    #define RGB_MATRIX_FRAMEBUFFER_EFFECTS
 */
    /* RGB Matrix config */
    /* 
    #define RGB_DI_PIN B1 */
    /* RGB Matrix effect */
    /* 
    #define ENABLE_RGB_MATRIX_ALPHAS_MODS
    #define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    #define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_BREATHING
    #define ENABLE_RGB_MATRIX_BAND_SAT
    #define ENABLE_RGB_MATRIX_BAND_VAL
    #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
    #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
    #define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
    #define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
    #define ENABLE_RGB_MATRIX_CYCLE_ALL
    #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    #define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    #define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    #define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    #define ENABLE_RGB_MATRIX_DUAL_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    #define ENABLE_RGB_MATRIX_RAINDROPS
    #define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    #define ENABLE_RGB_MATRIX_HUE_BREATHING
    #define ENABLE_RGB_MATRIX_HUE_PENDULUM
    #define ENABLE_RGB_MATRIX_HUE_WAVE
    #define ENABLE_RGB_MATRIX_PIXEL_RAIN
    #define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
// enabled only if RGB_MATRIX_FRAMEBUFFER_EFFECTS is defined
    #define ENABLE_RGB_MATRIX_TYPING_HEATMAP
    #define ENABLE_RGB_MATRIX_DIGITAL_RAIN
// enabled only of RGB_MATRIX_KEYPRESSES or RGB_MATRIX_KEYRELEASES is defined
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    #define ENABLE_RGB_MATRIX_SPLASH
    #define ENABLE_RGB_MATRIX_MULTISPLASH
    #define ENABLE_RGB_MATRIX_SOLID_SPLASH
    #define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#endif
*/