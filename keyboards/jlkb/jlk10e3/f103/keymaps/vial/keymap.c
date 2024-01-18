#include "f103.h"

enum keycodes {
  LAYERS_DOWN = SAFE_RANGE,
  LAYERS_UP,
  jltb,
  jldu,
  jldu1,
  jldu2,
  jldu3
};

// 1st layer on the cycle
#define LAYER_CYCLE_START 0
// Last layer on the cycle
#define LAYER_CYCLE_END   16

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT(
		jldu, jldu1, jldu2, jldu3, 
		jldu1, jldu1, jldu1, jldu1, 
		jldu1, jldu1, jldu1, jldu1, 
		jldu1, jldu1, jldu1, jldu1, 
		jldu1, jldu1, jldu1, jldu1),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN), ENCODER_CCW_CW(KC_VOLD, KC_VOLU),    ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [1] =   { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),           ENCODER_CCW_CW(RGB_SAD, RGB_SAI),    ENCODER_CCW_CW(RGB_SAD, RGB_SAI)  },

    //                  旋钮 1                                     旋钮 2                                      旋钮 3               
};
#endif

// Add the behaviour of this new keycode
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // 下一层
  switch (keycode) {
    case LAYERS_DOWN:
      // Our logic will happen on presses, nothing is done on releases
      if (!record->event.pressed) { 
        // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
        return false;
      }

      uint8_t current_layer = get_highest_layer(layer_state);

      // Check if we are within the range, if not quit
      if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
        return false;
      }

      uint8_t next_layer = current_layer + 1;
      if (next_layer > LAYER_CYCLE_END) {
          next_layer = LAYER_CYCLE_START;
      }
      layer_move(next_layer);
      return false;

    // Process other keycodes normally

     return true;
     break;
   // 上一层  
      case LAYERS_UP:
      // Our logic will happen on presses, nothing is done on releases
      if (!record->event.pressed) { 
        // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
        return false;
      }

      uint8_t current1_layer = get_highest_layer(layer_state);

      // Check if we are within the range, if not quit
      if (current1_layer > LAYER_CYCLE_END || current1_layer < LAYER_CYCLE_START) {
        return false;
      }

      uint8_t previous_layer = current1_layer - 1;
      if (previous_layer > LAYER_CYCLE_END) {
          previous_layer = LAYER_CYCLE_START;
      }
      layer_move(previous_layer);
      return false;

    // Process other keycodes normally
     default:
     return true;
     break;
     
 case jltb:
        if (record->event.pressed) {
            SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_R) SS_UP(X_LGUI) SS_DELAY(100) "https://jlkb.taobao.com" SS_TAP(X_ENTER) SS_TAP(X_ENTER));
        }
        
    return true;
    break;
    
 case jldu:
        if (record->event.pressed) {
            tap_code(KC_L);
        }else {
            tap_code(KC_K); 
      }
    return false;
    break;
    
    
 case jldu1:
        if (record->event.pressed) {
            SEND_STRING("ll");
        }else {
            tap_code(KC_K);  
      }
    return false;
    break;
    
    
 case jldu2:
        if (record->event.pressed) {
            SEND_STRING("lll");
        }else {
            tap_code(KC_K);  
      }
    return false;
    break;
    
    
 case jldu3:
        if (record->event.pressed) {
            SEND_STRING("llll");
        }else {
            tap_code(KC_K);   
      }
    return false;
    break;
  }
}

