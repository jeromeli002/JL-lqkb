//#include "f103.h"
#include QMK_KEYBOARD_H
#include "joystick.h"
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
		QK_BOOT, EE_CLR, QK_RBT, KC_TRNS,
		RESET, KC_TRNS, KC_TRNS, KC_TRNS,
		RESET, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		QK_BOOT, EE_CLR, QK_RBT, KC_TRNS,
		RESET, KC_TRNS, KC_TRNS, KC_TRNS,
		RESET, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] =   { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN),    ENCODER_CCW_CW(KC_VOLD, KC_VOLU) ,    ENCODER_CCW_CW(KC_VOLD, KC_VOLU) ,    ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [1] =   { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),              ENCODER_CCW_CW(RGB_SAD, RGB_SAI) ,    ENCODER_CCW_CW(KC_VOLD, KC_VOLU) ,    ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },

    //                  旋钮 1                                     旋钮 2                                       旋钮 3                                旋钮 4                  
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

void matrix_scan_user(void) {
	  // Up
  	if (!arrows[0] && analogReadPin(A1) - 512 > actuation) {
  			arrows[0] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 1,0);
  			register_code16(keycode);
  	} else if (arrows[0] &&  analogReadPin(A1) - 512 < actuation) {
  			arrows[0] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 1,0);
  			unregister_code16(keycode);
  	}
		// Down
  	if (!arrows[1] && analogReadPin(A1) - 512 < -actuation) {
  			arrows[1] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 2,0);
  			register_code16(keycode);
  	}	else if (arrows[1] && analogReadPin(A1) - 512 > -actuation) {
  			arrows[1] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 2,0);
  			unregister_code16(keycode);
  	}
    // Left
  	if (!arrows[2] && analogReadPin(A2) - 512 > actuation) {
  			arrows[2] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 2,1);
  			register_code16(keycode);
  	} else if (arrows[2] &&  analogReadPin(A2) - 512 < actuation) {
  			arrows[2] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 2,1);
  			unregister_code16(keycode);
  	}
    // Right
  	if (!arrows[3] && analogReadPin(A2) - 512 < -actuation) {
  			arrows[3] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,1);
  			register_code16(keycode);
  	} else if (arrows[3] && analogReadPin(A2) - 512 > -actuation) {
  			arrows[3] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,1);
  			unregister_code16(keycode);
  	}
}

// Joystick config
joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    [0] = JOYSTICK_AXIS_VIRTUAL,
    [1] = JOYSTICK_AXIS_VIRTUAL
};
/*
void matrix_scan_user(void) {
    int16_t val = (((uint32_t)timer_read() % 5000 - 2500) * 255) / 5000;
    joystick_set_axis(1, val);
}

//joystick config
joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    [0] = JOYSTICK_AXIS_IN(A1, 1023, 512, 0),
    [1] = JOYSTICK_AXIS_IN(A2, 0, 512, 1023)
};
*/