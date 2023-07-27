#include "f401.h"
enum keycodes {
  LAYERS_DOWN = SAFE_RANGE,
  LAYERS_UP
};

// 1st layer on the cycle
#define LAYER_CYCLE_START 0
// Last layer on the cycle
#define LAYER_CYCLE_END   16


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT(
		KC_TRNS, KC_ESCAPE, KC_2, KC_4, KC_6, KC_8, KC_0, KC_EQUAL, DIAL_R,
		KC_1, KC_TRNS, KC_3, KC_5, KC_7, KC_9, KC_MINUS, KC_BSPACE, DIAL_BUT,
		KC_TAB, KC_W, KC_TRNS, KC_R, KC_Y, KC_I, KC_P, KC_TRNS, DIAL_L,
		KC_Q, KC_E, KC_T, KC_TRNS, KC_U, KC_O, KC_LBRACKET, KC_HOME, KC_RBRACKET,
		KC_CAPSLOCK, KC_S, KC_F, KC_H, KC_TRNS, KC_K, KC_SCOLON, KC_PGUP, KC_BSLASH,
		KC_A, KC_D, KC_G, KC_J, KC_L, KC_TRNS, KC_QUOTE, KC_PGDOWN, KC_ENTER,
		KC_LSHIFT, KC_X, KC_V, KC_N, KC_COMMA, KC_SLASH, KC_TRNS, KC_END, KC_RSHIFT,
		KC_Z, KC_C, KC_B, KC_M, KC_RALT, KC_DOT, KC_RCTRL, KC_TRNS, LT(1, KC_APPLICATION),
		KC_LCTRL, KC_LGUI, KC_LALT, KC_SPACE, KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_ESCAPE, KC_F2, KC_F4, KC_F6, KC_F8, KC_F10, KC_F12, KC_MFFD,
		KC_F1, KC_TRNS, KC_F3, KC_F5, KC_F7, KC_F9, KC_F11, KC_DELETE, KC_MPLY,
		KC_TAB, KC_W, KC_TRNS, KC_R, KC_Y, KC_I, KC_P, KC_TRNS, KC_MRWD,
		KC_Q, KC_E, KC_T, KC_TRNS, KC_U, KC_O, KC_LBRACKET, RGB_RMOD, KC_RBRACKET,
		KC_CAPSLOCK, KC_S, KC_F, KC_H, KC_TRNS, KC_K, KC_SCOLON, RGB_MOD, KC_BSLASH,
		KC_A, KC_D, KC_G, KC_J, KC_L, KC_TRNS, KC_QUOTE, RGB_VAI, KC_ENTER,
		KC_LSHIFT, KC_X, KC_V, KC_N, KC_COMMA, KC_SLASH, KC_TRNS, RGB_VAD, KC_RSHIFT,
		KC_Z, KC_C, KC_B, KC_M, KC_RALT, KC_DOT, KC_RCTRL, KC_TRNS, KC_TRNS,
		KC_LCTRL, KC_LGUI, KC_LALT, KC_SPACE, KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)

};


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
    
  }
}