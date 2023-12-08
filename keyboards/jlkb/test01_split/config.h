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
#define MATRIX_ROW_PINS { A6, A7, A8 }
#define MATRIX_COL_PINS { A0, A1, A2 }
#define MATRIX_ROW_PINS_RIGHT { A6, A7, A8 }
#define MATRIX_COL_PINS_RIGHT { A3, A4, A5 }
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

#endif