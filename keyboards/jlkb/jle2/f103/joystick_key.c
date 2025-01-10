#include "analog.h"

//摇杆映射按键相关
static int actuation = 256;       // 触发点，即摇杆偏移多少开始触发按键 (0-511)
static int hysteresis = 50;        // 滞后量，防止抖动，调整此值以达到最佳效果
bool arrows[4];

static pin_t joystick_axes_x_pin = JOYSTICK_KEY_X_PIN;  //X轴引脚
static pin_t joystick_axes_y_pin = JOYSTICK_KEY_Y_PIN;  //Y轴引脚

// 定义一个结构体来表示按键在矩阵键盘中的位置
typedef struct {
    uint8_t row; // 行
    uint8_t col; // 列
} 
// 定义四个方向按键在矩阵键盘中的位置
matrix_pos_t;
matrix_pos_t joykey_xp = JOYSTICK_KEY_PX_POS;// 表示第 0 行第 2 列
matrix_pos_t joykey_xn = JOYSTICK_KEY_NX_POS;
matrix_pos_t joykey_yp = JOYSTICK_KEY_PY_POS; 
matrix_pos_t joykey_yn = JOYSTICK_KEY_NY_POS;

void matrix_scan_user(void) {
	  // 上(X 轴正方向)
  	if (!arrows[0] && analogReadPin(joystick_axes_x_pin) - 512 > actuation + hysteresis) {
  			arrows[0] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_xp.row, joykey_xp.col);
  			register_code16(keycode);
  	} else if (arrows[0] &&  analogReadPin(joystick_axes_x_pin) - 512 < actuation - hysteresis) {
  			arrows[0] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_xp.row, joykey_xp.col);
  			unregister_code16(keycode);
  	}
		// 下(X 轴正负向)
  	if (!arrows[1] && analogReadPin(joystick_axes_x_pin) - 512 < -actuation - hysteresis) {
  			arrows[1] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_xn.row, joykey_xn.col);
  			register_code16(keycode);
  	}	else if (arrows[1] && analogReadPin(joystick_axes_x_pin) - 512 > -actuation + hysteresis) {
  			arrows[1] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_xn.row, joykey_xn.col);
  			unregister_code16(keycode);
  	}
    // Left
  	if (!arrows[2] && analogReadPin(joystick_axes_y_pin) - 512 > actuation + hysteresis) {
  			arrows[2] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_yp.row, joykey_yp.col);
  			register_code16(keycode);
  	} else if (arrows[2] &&  analogReadPin(joystick_axes_y_pin) - 512 < actuation - hysteresis) {
  			arrows[2] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_yp.row, joykey_yp.col);
  			unregister_code16(keycode);
  	}
    // Right
  	if (!arrows[3] && analogReadPin(joystick_axes_y_pin) - 512 < -actuation - hysteresis) {
  			arrows[3] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_yn.row, joykey_yn.col);
  			register_code16(keycode);
  	} else if (arrows[3] && analogReadPin(joystick_axes_y_pin) - 512 > -actuation + hysteresis) {
  			arrows[3] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_yn.row, joykey_yn.col);
  			unregister_code16(keycode);
  	}
}

// Joystick config
joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    [0] = JOYSTICK_AXIS_VIRTUAL,
    [1] = JOYSTICK_AXIS_VIRTUAL
};
