#include QMK_KEYBOARD_H


// 定义GPIO引脚
// 这些定义通常放在 config.h 中，但为了示例清晰，这里直接写出
// 请根据您的实际硬件连接和STM32F401RCT6的数据手册确认正确的GPIO端口和引脚
#define PIN_A8   A8   // 假设PA8对应A8
#define PIN_C9   C9   // 假设PC9对应C9
#define PIN_C8   C8   // 假设PC8对应C8

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
  JLA,
  JLB,
  JLC
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT(
		KC_A, KC_B, KC_C, KC_D, KC_E, 
		KC_F, KC_G, KC_H, KC_I, KC_J, 
		JLB, JLA, JLC, KC_N, KC_O, 
		KC_P, KC_Q, KC_R, KC_S, KC_T, 
		KC_U, KC_V, KC_W, KC_X, KC_Y),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] =   { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) , ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },
    [1] =   { ENCODER_CCW_CW(RGB_HUD, RGB_HUI)           , ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },

    //                  旋钮 1                                          旋钮 2                               
};
#endif

// 添加新的按键
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
        switch (keycode) {
            case JLA: // 假设您定义了一个自定义按键来切换到B状态
                if (record->event.pressed) {
                if (current_keyboard_state != STATE_A) {
                    // 切换到B状态：A8低电平，C9高电平，C8低电平
                    gpio_write_pin_high(PIN_A8);
                    gpio_write_pin_low(PIN_C9);
                    gpio_write_pin_low(PIN_C8);
                    current_keyboard_state = STATE_A;
                    // dprintf("Switched to State B\n"); // 可选：调试信息
                }
                tap_code(KC_A); // 无论是否切换状态，都发送KC_B
                }
                return false; // 不将此按键事件发送到常规处理流程

            case JLB: // 假设您定义了一个自定义按键来切换到C状态
                if (record->event.pressed) {
                if (current_keyboard_state != STATE_B) {
                    // 切换到C状态：A8低电平，C9低电平，C8高电平
                    gpio_write_pin_low(PIN_A8);
                    gpio_write_pin_high(PIN_C9);
                    gpio_write_pin_low(PIN_C8);
                    current_keyboard_state = STATE_B;
                    // dprintf("Switched to State C\n"); // 可选：调试信息
                }
                tap_code(KC_B); // 无论是否切换状态，都发送KC_C
                }
                return false; // 不将此按键事件发送到常规处理流程

            case JLC: // 假设您定义了一个自定义按键来切换回A状态
                if (record->event.pressed) {
                if (current_keyboard_state != STATE_C) {
                    // 切换回A状态：A8高电平，C9低电平，C8低电平
                    gpio_write_pin_low(PIN_A8);
                    gpio_write_pin_low(PIN_C9);
                    gpio_write_pin_high(PIN_C8);
                    current_keyboard_state = STATE_C;
                    // dprintf("Switched to State A\n"); // 可选：调试信息
                }
                tap_code(KC_C); // 无论是否切换状态，都发送KC_A
                }
                return false; // 不将此按键事件发送到常规处理流程
        }
    return true; // 继续处理其他按键
}




