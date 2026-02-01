#include QMK_KEYBOARD_H
// #include "joystick_key.c"
#include "jloled.c"
#include "jloled.h"
#include "oled.c" //层信息图像显
#include "custom_matrix_on.c"   // 处理接收到的原始数据

#include "config.h"
#include "ws2812.h"
#include "color.h"
#include "bhq_common.h"
#include "wireless.h"

enum custom_keycodes {
  LAYERS_DOWN = QK_KB_0,
  LAYERS_UP,
  jltb,
  jld6u7
};

#define HIGHEST_LAYER 15 //最高层数 0开始算起默认15(16层)
static uint8_t current_layer = 0; //默认0层开始

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT(
		BLE_SW1, BLE_SW2, BLE_SW3,  RF_TOG, KC_M, KC_N, KC_O, KC_P, 
		KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, 
		KC_I, KC_J, KC_K, KC_L, KC_L, KC_C, KC_C, KC_Q, 
		KC_R, KC_S, KC_T, KC_U, KC_U, KC_W, KC_X, KC_Y, 
		KC_Z, KC_Z, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, 
		KC_Z, KC_9, KC_TRNS, KC_TRNS, KC_TRNS, KC_F, KC_F, KC_TRNS, 
		KC_TRNS, KC_G, KC_TRNS, KC_TRNS, KC_TRNS, KC_G, KC_G, KC_TRNS, 
		KC_TRNS, KC_H, KC_TRNS, KC_TRNS, KC_TRNS, KC_H, KC_H, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   { ENCODER_CCW_CW(KC_A, KC_A),    ENCODER_CCW_CW(KC_A, KC_A) ,    ENCODER_CCW_CW(KC_A, KC_A) ,    ENCODER_CCW_CW(KC_A, KC_A),    ENCODER_CCW_CW(KC_A, KC_A),    ENCODER_CCW_CW(KC_A, KC_A)  },
    [1] =   { ENCODER_CCW_CW(KC_A, KC_A),    ENCODER_CCW_CW(KC_A, KC_A) ,    ENCODER_CCW_CW(KC_A, KC_A) ,    ENCODER_CCW_CW(KC_A, KC_A),    ENCODER_CCW_CW(KC_A, KC_A),    ENCODER_CCW_CW(KC_A, KC_A)  },

    //                  旋钮 1                                     旋钮 2                                       旋钮 3                                旋钮 4                                        旋钮 5                                     旋钮 6               
};
#endif

bool via_command_kb(uint8_t *data, uint8_t length) {
    return via_command_bhq(data, length);
}

// 添加新的按键
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_bhq(keycode, record)) {
        return false;
    }
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

