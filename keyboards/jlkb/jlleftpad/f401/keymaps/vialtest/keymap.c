#include QMK_KEYBOARD_H
#include "raw_hid.h"

// 定义GPIO引脚
// 这些定义通常放在 config.h 中，但为了示例清晰，这里直接写出
// 请根据您的实际硬件连接和STM32F401RCT6的数据手册确认正确的GPIO端口和引脚
#define PIN_A8 A8
#define PIN_C9 C9
#define PIN_C8 C8

// 定义GPIO引脚数组，方便索引操作
static const pin_t gpio_pins[] = {PIN_A8, PIN_C9, PIN_C8};
#define NUM_GPIO_PINS (sizeof(gpio_pins) / sizeof(gpio_pins[0]))

// 定义状态枚举
typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D // 所有引脚均为低电平
} keyboard_state_t;

keyboard_state_t current_keyboard_state = STATE_D; // 初始状态为D（所有引脚低电平）

void setup_gpios(void) {
    // 设置所有相关引脚为输出模式
    for (uint8_t i = 0; i < NUM_GPIO_PINS; i++) {
        gpio_set_pin_output(gpio_pins[i]);
        // 初始时将所有引脚设置为低电平
        gpio_write_pin_low(gpio_pins[i]);
    }
}

// 每次键盘启动时调用
void matrix_init_user(void) {
    setup_gpios();
}

enum keycodes {
    LAYERS_DOWN = QK_KB_0,
    LAYERS_UP,
    jltb,
    jld6u7,
    modelA,
    modelB,
    modelC
};

#define HIGHEST_LAYER 7 // 最高层数 0开始算起默认15(16层)
static uint8_t current_layer = 0; // 默认0层开始

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_A, KC_B, KC_C, KC_D, KC_E,
        KC_F, KC_G, KC_H, KC_I, KC_J,
        modelC, modelB, modelA, KC_N, KC_O,
        KC_P, KC_Q, KC_R, KC_S, KC_T,
        KC_U, KC_V, KC_W, KC_X, KC_Y
    ),
    [1] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN), ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    [1] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI), ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    // 旋钮 1                                 旋钮 2
};
#endif

// 发送一个32字节数据X为0xXX则拉高A8
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (length == 32) {
        // 无论data[3], data[4], data[5]是否为0x00，都先将所有相关引脚设置为低电平
        for (uint8_t i = 0; i < NUM_GPIO_PINS; i++) {
            gpio_write_pin_low(gpio_pins[i]);
        }

        // 只有当data[3], data[4], data[5]都为0x00时，才根据data[6]进行高电平设置
        if (data[3] == 0x00 && data[4] == 0x00 && data[5] == 0x00) {
            switch (data[6]) {
                case 0x01:
                    gpio_write_pin_high(PIN_A8);
                    current_keyboard_state = STATE_A;
                    break;
                case 0x02:
                    gpio_write_pin_high(PIN_C9);
                    current_keyboard_state = STATE_B;
                    break;
                case 0x03:
                    gpio_write_pin_high(PIN_C8);
                    current_keyboard_state = STATE_C;
                    break;
                default:
                    current_keyboard_state = STATE_D; // 其他data[6]值或不满足条件时，所有引脚为低电平
                    break;
            }
        } else {
            current_keyboard_state = STATE_D; // data[3], data[4], data[5]不全为0x00时，所有引脚为低电平
        }
    }
}

// 添加新的按键
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true; // 只处理按键按下事件
    }

    switch (keycode) {
        case LAYERS_DOWN: // 下一层
            current_layer = (current_layer == HIGHEST_LAYER) ? 0 : (current_layer + 1);
            layer_clear();
            layer_on(current_layer);
            return false;

        case LAYERS_UP: // 上一层
            current_layer = (current_layer == 0) ? HIGHEST_LAYER : (current_layer - 1);
            layer_clear();
            layer_on(current_layer);
            return false;

        case jltb: // 打开淘宝店
            SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_R) SS_UP(X_LGUI) SS_DELAY(100) "https://jlkb.taobao.com" SS_TAP(X_ENTER) SS_TAP(X_ENTER));
            return false;

        case jld6u7: // 按下6层抬起7层
            tap_code16(keymap_key_to_keycode(6, record->event.key));
            return false;

        case modelA: // 切换到A状态
            if (current_keyboard_state != STATE_A) {
                // 将所有引脚设置为低电平
                for (uint8_t i = 0; i < NUM_GPIO_PINS; i++) {
                    gpio_write_pin_low(gpio_pins[i]);
                }
                gpio_write_pin_high(PIN_A8);
                current_keyboard_state = STATE_A;
            }
            tap_code16(KC_A);
            return false;

        case modelB: // 切换到B状态
            if (current_keyboard_state != STATE_B) {
                // 将所有引脚设置为低电平
                for (uint8_t i = 0; i < NUM_GPIO_PINS; i++) {
                    gpio_write_pin_low(gpio_pins[i]);
                }
                gpio_write_pin_high(PIN_C9);
                current_keyboard_state = STATE_B;
            }
            tap_code16(KC_B);
            return false;

        case modelC: // 切换到C状态
            if (current_keyboard_state != STATE_C) {
                // 将所有引脚设置为低电平
                for (uint8_t i = 0; i < NUM_GPIO_PINS; i++) {
                    gpio_write_pin_low(gpio_pins[i]);
                }
                gpio_write_pin_high(PIN_C8);
                current_keyboard_state = STATE_C;
            }
            tap_code16(KC_C);
            return false;
    }
    return true; // 继续处理其他按键
}