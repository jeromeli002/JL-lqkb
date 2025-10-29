#include QMK_KEYBOARD_H
#include "custom_matrix_light.h" // 包含你的驱动头文件


enum custom_keycodes {
    QK_LED_ON = QK_KB_0,  // 定义自定义键码，用于测试点亮灯
    QK_LED_OFF,
    ML_OFF                   // 定义自定义键码，用于测试熄灭所有灯
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    LAYOUT(
        KC_A, KC_B, QK_BOOT,
        QK_LED_ON, QK_LED_OFF, ML_OFF),

    LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS)

};

// 键盘初始化函数
void keyboard_post_init(void) {
    custom_matrix_light_init(); // 初始化矩阵灯驱动
}

// 主循环任务函数，QMK会不断调用这个函数
void matrix_scan_user(void) {
    custom_matrix_light_task(); // 调用驱动的任务函数来刷新显示和处理动画
}

// 按键事件处理函数
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    char char_to_display = 0xFF; // 默认值设置为 0xFF，以便在未匹配时显示笑脸

    bool is_shift_active = (get_mods() & MOD_MASK_SHIFT);

    if (record->event.pressed) {
        #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
        custom_matrix_light_stop_rain(); // 当按键按下时，停止下雨效果
        #endif
        
        // 检查自定义键码
        switch (keycode) {
            case QK_LED_ON:
                // 示例：点亮 (1,2) 和 (3,4) 处的灯
                // 现在 custom_matrix_light_set_pixels 不再自动清空矩阵
                // 如果你希望每次点亮时都只显示这些灯，需要先清空自定义像素层
                custom_matrix_light_clear_all(); // 清空所有显示，确保只显示这些灯
                static const matrix_pixel_t points_to_light[] = {{1, 2}, {3, 4}};
                custom_matrix_light_set_pixels(points_to_light, ARRAY_SIZE(points_to_light), true);
                return false; // 返回 false，表示这个键不传递给固件的默认处理
            
            case QK_LED_OFF:
                // 示例：熄灭 (1,2) 处的灯
                // 现在这个操作只会熄灭指定像素，不会影响其他层或未指定的像素
                static const matrix_pixel_t points_to_extinguish[] = {{1, 2}};
                custom_matrix_light_set_pixels(points_to_extinguish, ARRAY_SIZE(points_to_extinguish), false);
                return false; // 返回 false，表示这个键不传递给固件的默认处理
            
            case ML_OFF:
                custom_matrix_light_clear_all(); // 调用函数关闭所有灯（包括字符、像素和雨滴）
                return false; // 返回 false，表示这个键不传递给固件的默认处理
            
        }

        // 对于常规按键，更新字符显示层
        switch (keycode) {
            // --- 字母键 (区分大小写) ---
            case KC_A: char_to_display = is_shift_active ? 'A' : 'a'; break;
            case KC_B: char_to_display = is_shift_active ? 'B' : 'b'; break;
            case KC_C: char_to_display = is_shift_active ? 'C' : 'c'; break;
            case KC_D: char_to_display = is_shift_active ? 'D' : 'd'; break;
            case KC_E: char_to_display = is_shift_active ? 'E' : 'e'; break;
            case KC_F: char_to_display = is_shift_active ? 'F' : 'f'; break;
            case KC_G: char_to_display = is_shift_active ? 'G' : 'g'; break;
            case KC_H: char_to_display = is_shift_active ? 'H' : 'h'; break;
            case KC_I: char_to_display = is_shift_active ? 'I' : 'i'; break;
            case KC_J: char_to_display = is_shift_active ? 'J' : 'j'; break;
            case KC_K: char_to_display = is_shift_active ? 'K' : 'k'; break;
            case KC_L: char_to_display = is_shift_active ? 'L' : 'l'; break;
            case KC_M: char_to_display = is_shift_active ? 'M' : 'm'; break;
            case KC_N: char_to_display = is_shift_active ? 'N' : 'n'; break;
            case KC_O: char_to_display = is_shift_active ? 'O' : 'o'; break;
            case KC_P: char_to_display = is_shift_active ? 'P' : 'p'; break;
            case KC_Q: char_to_display = is_shift_active ? 'Q' : 'q'; break;
            case KC_R: char_to_display = is_shift_active ? 'R' : 'r'; break;
            case KC_S: char_to_display = is_shift_active ? 'S' : 's'; break;
            case KC_T: char_to_display = is_shift_active ? 'T' : 't'; break;
            case KC_U: char_to_display = is_shift_active ? 'U' : 'u'; break;
            case KC_V: char_to_display = is_shift_active ? 'V' : 'v'; break;
            case KC_W: char_to_display = is_shift_active ? 'W' : 'w'; break;
            case KC_X: char_to_display = is_shift_active ? 'X' : 'x'; break;
            case KC_Y: char_to_display = is_shift_active ? 'Y' : 'y'; break;
            case KC_Z: char_to_display = is_shift_active ? 'Z' : 'z'; break;

            // --- 主键盘区数字和带 Shift 的符号键 ---
            case KC_0: char_to_display = is_shift_active ? ')' : '0'; break;
            case KC_1: char_to_display = is_shift_active ? '!' : '1'; break;
            case KC_2: char_to_display = is_shift_active ? '@' : '2'; break;
            case KC_3: char_to_display = is_shift_active ? '#' : '3'; break;
            case KC_4: char_to_display = is_shift_active ? '$' : '4'; break;
            case KC_5: char_to_display = is_shift_active ? '%' : '5'; break;
            case KC_6: char_to_display = is_shift_active ? '^' : '6'; break;
            case KC_7: char_to_display = is_shift_active ? '&' : '7'; break;
            case KC_8: char_to_display = is_shift_active ? '*' : '8'; break;
            case KC_9: char_to_display = is_shift_active ? '(' : '9'; break;

            // --- 数字键盘区 (Num Pad) 数字键 ---
            case KC_P0: char_to_display = '0'; break;
            case KC_P1: char_to_display = '1'; break;
            case KC_P2: char_to_display = '2'; break;
            case KC_P3: char_to_display = '3'; break;
            case KC_P4: char_to_display = '4'; break;
            case KC_P5: char_to_display = '5'; break;
            case KC_P6: char_to_display = '6'; break;
            case KC_P7: char_to_display = '7'; break;
            case KC_P8: char_to_display = '8'; break;
            case KC_P9: char_to_display = '9'; break;
            case KC_PDOT: char_to_display = '.'; break; // 数字键盘小数点
            case KC_PSLS: char_to_display = '/'; break; // 数字键盘除号
            case KC_PAST: char_to_display = '*'; break; // 数字键盘乘号
            case KC_PMNS: char_to_display = '-'; break; // 数字键盘减号
            case KC_PPLS: char_to_display = '+'; break; // 数字键盘加号

            // --- 其他常用符号键 (需要根据你的键盘布局和常用符号进行补充) ---
            case KC_MINUS:      char_to_display = is_shift_active ? '_' : '-'; break;
            case KC_EQUAL:      char_to_display = is_shift_active ? '+' : '='; break;
            case KC_LBRC:       char_to_display = is_shift_active ? '{' : '['; break;
            case KC_RBRC:       char_to_display = is_shift_active ? '}' : ']'; break;
            case KC_BSLS:       char_to_display = is_shift_active ? '|' : '\\'; break;
            case KC_SCLN:       char_to_display = is_shift_active ? ':' : ';'; break;
            case KC_QUOTE:      char_to_display = is_shift_active ? '"' : '\''; break;
            case KC_GRAVE:      char_to_display = is_shift_active ? '~' : '`'; break;
            case KC_COMMA:      char_to_display = is_shift_active ? '<' : ','; break;
            case KC_DOT:        char_to_display = is_shift_active ? '>' : '.'; break;
            case KC_SLASH:      char_to_display = is_shift_active ? '?' : '/'; break;

            // --- 特殊键 ---
            case KC_SPC: char_to_display = ' '; break; // 空格键
            case KC_ENT: char_to_display = '\n'; break; // 回车键 (可能需要自定义显示)
            case KC_BSPC: char_to_display = '\b'; break; // 退格键 (可能需要自定义显示)
            case KC_TAB: char_to_display = '\t'; break; // Tab 键 (可能需要自定义显示)


            // 对于所有未明确处理的键，默认显示笑脸（因为 char_to_display 初始化为 0xFF）
            default:
                char_to_display = 0xFF; // 未映射的键码，传递特殊值以显示笑脸
                break;
        }

        // 调用驱动函数来设置要显示的字符/图案
        custom_matrix_light_set_char(char_to_display); // 这将更新字符显示层
    } else {
        // 当按键释放时，可以根据需要恢复下雨效果或显示其他默认图案
        // custom_matrix_light_start_rain(); // 如果希望按键释放后自动恢复
    }
    return true; // 继续处理按键
}