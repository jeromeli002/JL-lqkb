#include "analog.h"

static int actuation = 256;
static int hysteresis = 50;

static pin_t joystick_axes_x_pin[] = JOYSTICK_KEY_X_PIN;
static pin_t joystick_axes_y_pin[] = JOYSTICK_KEY_Y_PIN;

#define NUMBER_OF_JOYSTICKS (sizeof(joystick_axes_x_pin) / sizeof(pin_t)) 

typedef struct {
    uint8_t row;
    uint8_t col;
} matrix_pos_t;

matrix_pos_t joykey_xp[] = JOYSTICK_KEY_PX_POS;
matrix_pos_t joykey_xn[] = JOYSTICK_KEY_NX_POS;
matrix_pos_t joykey_yp[] = JOYSTICK_KEY_PY_POS;
matrix_pos_t joykey_yn[] = JOYSTICK_KEY_NY_POS;

bool arrows[4 * NUMBER_OF_JOYSTICKS] = {false}; 

void update_arrow_state(int joystick_index, int value, bool* state, matrix_pos_t* key_pos) {
    uint16_t keycode = KC_NO;
    uint8_t layer = biton32(layer_state);
    while ((keycode == KC_TRNS) && (layer >= 0)) {
        keycode = dynamic_keymap_get_keycode(layer_state, key_pos[joystick_index].row, key_pos[joystick_index].col);
        layer_state --;
    }

    bool should_press = value > actuation + hysteresis;
    bool should_release = value < actuation - hysteresis;

    if (!(*state) && should_press) {
        *state = true;
        
        keyrecord_t record;
        record.event.key.row = key_pos[joystick_index].row;
        record.event.key.col = key_pos[joystick_index].col;
        record.keycode = keycode; 
        record.event.pressed = true;
        record.event.time = timer_read();
        
        process_record(&record);
        
    } else if (*state && should_release) {
        *state = false;
        
        keyrecord_t record;
        record.event.key.row = key_pos[joystick_index].row;
        record.event.key.col = key_pos[joystick_index].col;
        record.keycode = keycode; 
        record.event.pressed = false;
        record.event.time = timer_read();
        
        process_record(&record);
    }
}

void matrix_scan_kb(void) {
    static int x_value[NUMBER_OF_JOYSTICKS]; 
    static int y_value[NUMBER_OF_JOYSTICKS]; 

    for (uint8_t i = 0; i < NUMBER_OF_JOYSTICKS; i++) {
        x_value[i] = analogReadPin(joystick_axes_x_pin[i]);
        y_value[i] = analogReadPin(joystick_axes_y_pin[i]);

        update_arrow_state(i, 
                           x_value[i] - 512, 
                           &arrows[i * 4 + 0], 
                           joykey_xp);

        update_arrow_state(i, 
                           -(x_value[i] - 512), 
                           &arrows[i * 4 + 1], 
                           joykey_xn);

        update_arrow_state(i, 
                           y_value[i] - 512, 
                           &arrows[i * 4 + 2], 
                           joykey_yp);

        update_arrow_state(i, 
                           -(y_value[i] - 512), 
                           &arrows[i * 4 + 3], 
                           joykey_yn);
    }
}