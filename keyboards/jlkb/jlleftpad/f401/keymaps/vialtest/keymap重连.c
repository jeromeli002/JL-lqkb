#include QMK_KEYBOARD_H
#include "raw_hid.h"

// 定义GPIO引脚
// 这些定义通常放在 config.h 中，但为了示例清晰，这里直接写出
// 请根据您的实际硬件连接和STM32F401RCT6的数据手册确认正确的GPIO端口和引脚
#define PIN_A8   A8    // 假设PA8对应A8
#define PIN_C9   C9    // 假设PC9对应C9
#define PIN_C8   C8    // 假设PC8对应C8


// 定义状态枚举（可选，但有助于代码可读性）
typedef enum {
    STATE_A,
    STATE_B,
    STATE_C
} keyboard_state_t;

keyboard_state_t current_keyboard_state = STATE_A; // 初始状态为A

void setup_gpios(void) {
    // 设置所有相关引脚为输出模式
    gpio_set_pin_output(PIN_A8);
    gpio_set_pin_output(PIN_C9);
    gpio_set_pin_output(PIN_C8);

    // 设置初始状态：A8高电平，C8、C9低电平
    gpio_write_pin_high(PIN_A8);
    gpio_write_pin_low(PIN_C9);
    gpio_write_pin_low(PIN_C8);
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

#define HIGHEST_LAYER 7 //最高层数 0开始算起默认15(16层)
static uint8_t current_layer = 0; //默认0层开始


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_A, KC_B, KC_C, KC_D, KC_E,
        KC_F, KC_G, KC_H, KC_I, KC_J,
        modelB, modelA, QK_BOOT, KC_N, KC_O,
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
    [0] = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) , ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },
    [1] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI)           , ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },

    //                         旋钮 1                                           旋钮 2
};
#endif

//唤醒或重新连接执行
void suspend_wakeup_init_user(void) {
    gpio_write_pin_high(PIN_A8);
    gpio_write_pin_low(PIN_C9);
    gpio_write_pin_low(PIN_C8);
    current_keyboard_state = STATE_A;
}

// 添加新的按键
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LAYERS_DOWN: // 下一层
            if (record->event.pressed) {
                if (current_layer == HIGHEST_LAYER) {
                    current_layer = 0;
                } else {
                    current_layer++;
                }
                layer_clear();
                layer_on(current_layer);
            }
            return false;

        case LAYERS_UP: // 上一层
            if (record->event.pressed) {
                if (current_layer == 0) {
                    current_layer = HIGHEST_LAYER;
                } else {
                    current_layer--;
                }
                layer_clear();
                layer_on(current_layer);
            }
            return false;

        case jltb: // 打开淘宝店
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_R) SS_UP(X_LGUI) SS_DELAY(100) "https://jlkb.taobao.com" SS_TAP(X_ENTER) SS_TAP(X_ENTER));
            }
            return false;

        case jld6u7: // 按下6层抬起7层
            if (record->event.pressed) {
                tap_code16(keymap_key_to_keycode(6, record->event.key));
            } else {
                tap_code16(keymap_key_to_keycode(7, record->event.key));
            }
            return false;

        case modelA: // 切换到A状态
            if (record->event.pressed) {
                if (current_keyboard_state != STATE_A) {
                    // 切换到A状态：A8高电平，C9低电平，C8低电平
                    gpio_write_pin_high(PIN_A8);
                    gpio_write_pin_low(PIN_C9);
                    gpio_write_pin_low(PIN_C8);
                    current_keyboard_state = STATE_A;
                    // dprintf("Switched to State A\n"); // 可选：调试信息
                }
                tap_code16(KC_A); // 无论是否切换状态，都发送KC_A
            }
            return false; // 不将此按键事件发送到常规处理流程

        case modelB: // 切换到B状态
            if (record->event.pressed) {
                if (current_keyboard_state != STATE_B) {
                    // 切换到B状态：A8低电平，C9高电平，C8低电平
                    gpio_write_pin_low(PIN_A8);
                    gpio_write_pin_high(PIN_C9);
                    gpio_write_pin_low(PIN_C8);
                    current_keyboard_state = STATE_B;
                    // dprintf("Switched to State B\n"); // 可选：调试信息
                }
                tap_code16(KC_B); // 无论是否切换状态，都发送KC_B
            }
            return false; // 不将此按键事件发送到常规处理流程

        case modelC: // 切换到C状态
            if (record->event.pressed) {
                if (current_keyboard_state != STATE_C) {
                    // 切换到C状态：A8低电平，C9低电平，C8高电平
                    gpio_write_pin_low(PIN_A8);
                    gpio_write_pin_low(PIN_C9);
                    gpio_write_pin_high(PIN_C8);
                    current_keyboard_state = STATE_C;
                    // dprintf("Switched to State C\n"); // 可选：调试信息
                }
                tap_code16(KC_C); // 无论是否切换状态，都发送KC_C
            }
            return false; // 不将此按键事件发送到常规处理流程
    }
    return true; // 继续处理其他按键
}

