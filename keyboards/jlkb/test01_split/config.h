#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0x3331
#define PRODUCT_ID      0x721C
#define DEVICE_VER      0x0001
#define MANUFACTURER    eniru
#define PRODUCT         test01_split
// #define DESCRIPTION     Keyboard

/* key matrix size */
#define MATRIX_ROWS 6   //the sum
#define MATRIX_COLS 3

/* key matrix pins */
#define MATRIX_ROW_PINS { B9, B8, B7 }
#define MATRIX_COL_PINS { A15, A14, A13 }
#define MATRIX_ROW_PINS_RIGHT { B5, B4, B3 }
#define MATRIX_COL_PINS_RIGHT { A10, A9, A8 }
// #define UNUSED_PINS


//  分体设置
#define SERIAL_USART_FULL_DUPLEX   // Enable full duplex operation mode.
#define SERIAL_USART_TX_PIN A9     // USART TX pin
#define SERIAL_USART_RX_PIN A10     // USART RX pin
#define MASTER_LEFT

// A0-10 A15       A11 A12是usb的
// B0 B1 B3-B15    B6 B7给oled

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* encoder pins */
// #define ENCODERS_PAD_A { B10, B12 }
// #define ENCODERS_PAD_B { B11, B13 }
// #define ENCODER_DIRECTION_FLIP
// #define ENCODER_RESOLUTIONS { 4, 4 }

/* oled */
// #define OLED_BRIGHTNESS 128
// #define OLED_TIMEOUT 30000

/* I2C */
// #define I2C1_CLOCK_SPEED 40000
// #define I2C1_DUTY_CYCLE FAST_DUTY_CYCLE_2


/* 1000Hz = 1 , 500Hz = 2 , default = 10(100Hz) */
#define USB_POLLING_INTERVAL_MS 1

#define DYNAMIC_KEYMAP_LAYER_COUNT 6

/* Set 0 if debouncing isn't needed */
// #define DEBOUNCING_DELAY 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* prevent stuck modifiers */
// #define PREVENT_STUCK_MODIFIERS


// #ifdef RGB_DI_PIN
// #define RGBLIGHT_ANIMATIONS
// #define RGBLED_NUM 0
// #define RGBLIGHT_HUE_STEP 8
// #define RGBLIGHT_SAT_STEP 8
// #define RGBLIGHT_VAL_STEP 8
// #endif

//  矩阵灯
    #define DRIVER_LED_TOTAL 18
    #define RGB_MATRIX_LED_COUNT 18
    #define RGB_DISABLE_AFTER_TIMEOUT 0          // number of ticks to wait until disabling effects
    #define RGB_DI_PIN B1 
    #define RGB_DISABLE_WHEN_USB_SUSPENDED true
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200
    #define RGB_MATRIX_STARTUP_VAL 80
    #define WS2812_PWM_PAL_MODE 2 
    #define RGB_MATRIX_CENTER {106, 32}
    #define RGB_MATRIX_KEYPRESSES
    #define RGB_MATRIX_FRAMEBUFFER_EFFECTS

    /* RGB Matrix config */
     
    /* RGB Matrix effect */
    
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