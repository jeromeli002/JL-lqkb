#include QMK_KEYBOARD_H
#include "f401.h"
// #include "oled.c"

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
		KC_Q, KC_W, KC_E, KC_R, KC_T, 
		MO(1), MO(2), MO(3), MO(5), MO(7), 
		KC_Z, KC_X, KC_C, KC_V, KC_B, 
		KC_U, KC_I, KC_O, KC_P, KC_LBRC, 
		KC_P1, KC_P2, KC_P3, KC_P4, KC_COMM), 

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		MO(1), MO(3), MO(4), MO(6), MO(8),
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		MO(3), MO(2), MO(5), MO(7), MO(9),
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		MO(4), MO(5), MO(3), MO(8), MO(10),
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

joystick_config_t joystick_axes[JOYSTICK_AXES_COUNT] = {
    [0] = JOYSTICK_AXIS_IN(A4, 0, 512, 1023),
    [1] = JOYSTICK_AXIS_IN(A5, 0, 512, 1023),
    [2] = JOYSTICK_AXIS_IN(A6, 0, 512, 1023),
    [3] = JOYSTICK_AXIS_IN(A7, 0, 512, 1023)
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