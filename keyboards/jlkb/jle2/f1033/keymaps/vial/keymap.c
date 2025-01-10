//#include "f103.h"
#include QMK_KEYBOARD_H
#include "analog.h"

void board_init(void) {
   //禁用JTAG-DP调试，启用A13、A14脚    
   AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG_Msk) | AFIO_MAPR_SWJ_CFG_DISABLE;
}

enum custom_keycodes {
  LAYERS_DOWN = QK_KB_0,
  LAYERS_UP,
  jltb,
  jld6u7
};

static int actuation = 256; // actuation point for arrows (0-511)
bool arrows[4];

#define HIGHEST_LAYER 15 //最高层数 0开始算起默认15(16层)
static uint8_t current_layer = 0; //默认0层开始

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT(
		KC_A, KC_B, KC_C, KC_D, 
		KC_E, KC_F, KC_G, KC_H),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};


#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   { ENCODER_CCW_CW(DIAL_L, DIAL_R),    ENCODER_CCW_CW(DIAL_L, DIAL_R)  },
    [1] =   { ENCODER_CCW_CW(KC_TRNS, KC_TRNS),  ENCODER_CCW_CW(DIAL_L, DIAL_R)  },

    //                  旋钮 1                                     旋钮 2                   
};
#endif

// 添加新的按键
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // 下一层
  switch (keycode) {
     case LAYERS_DOWN:
      if(record->event.pressed) {
      if (current_layer == HIGHEST_LAYER){
        current_layer=0;
      } else {
        current_layer++;
      }
      layer_clear();
      layer_on(current_layer);
    }
    return false;
    
   // 上一层  
      case LAYERS_UP:
      if(record->event.pressed) {
      if (current_layer == 0){
        current_layer=HIGHEST_LAYER;
      } else {
        current_layer--;
      }
      layer_clear();
      layer_on(current_layer);
    }
    return false;

    // 处理其他键
     default:
     return true;
     break;

    case jltb:
        if (record->event.pressed) {
            SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_R) SS_UP(X_LGUI) SS_DELAY(100) "https://jlkb.taobao.com" SS_TAP(X_ENTER) SS_TAP(X_ENTER));
        }
        break;
   
      //按下6层抬起7层    
     case jld6u7:
        if (record->event.pressed) {
           tap_code16(keymap_key_to_keycode(6, record->event.key));
        }else {
           tap_code16(keymap_key_to_keycode(7, record->event.key)); 
      }
    return false;
    break;
    
   // 下一个自定义键
   
  }
   return true;
}


static pin_t joystick_axes_x_pin = JOYSTICK_KEY_X_PIN;  //X轴引脚
static pin_t joystick_axes_y_pin = JOYSTICK_KEY_Y_PIN;  //Y轴引脚
typedef struct {
    uint8_t row; // 行
    uint8_t col; // 列
} 
matrix_pos_t;
matrix_pos_t joykey_xp = JOYSTICK_KEY_PX_POS ;// 表示第 0 行第 2 列
matrix_pos_t joykey_xn = JOYSTICK_KEY_NX_POS;
matrix_pos_t joykey_yp = JOYSTICK_KEY_PY_POS; 
matrix_pos_t joykey_yn = JOYSTICK_KEY_NY_POS;

void matrix_scan_user(void) {
	  // Up
  	if (!arrows[0] && analogReadPin(joystick_axes_x_pin) - 512 > actuation) {
  			arrows[0] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_xp.row, joykey_xp.col);
  			register_code16(keycode);
  	} else if (arrows[0] &&  analogReadPin(joystick_axes_x_pin) - 512 < actuation) {
  			arrows[0] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_xp.row, joykey_xp.col);
  			unregister_code16(keycode);
  	}
		// Down
  	if (!arrows[1] && analogReadPin(joystick_axes_x_pin) - 512 < -actuation) {
  			arrows[1] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_xn.row, joykey_xn.col);
  			register_code16(keycode);
  	}	else if (arrows[1] && analogReadPin(joystick_axes_x_pin) - 512 > -actuation) {
  			arrows[1] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_xn.row, joykey_xn.col);
  			unregister_code16(keycode);
  	}
    // Left
  	if (!arrows[2] && analogReadPin(joystick_axes_y_pin) - 512 > actuation) {
  			arrows[2] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_yp.row, joykey_yp.col);
  			register_code16(keycode);
  	} else if (arrows[2] &&  analogReadPin(joystick_axes_y_pin) - 512 < actuation) {
  			arrows[2] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_yp.row, joykey_yp.col);
  			unregister_code16(keycode);
  	}
    // Right
  	if (!arrows[3] && analogReadPin(joystick_axes_y_pin) - 512 < -actuation) {
  			arrows[3] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), joykey_yn.row, joykey_yn.col);
  			register_code16(keycode);
  	} else if (arrows[3] && analogReadPin(joystick_axes_y_pin) - 512 > -actuation) {
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
