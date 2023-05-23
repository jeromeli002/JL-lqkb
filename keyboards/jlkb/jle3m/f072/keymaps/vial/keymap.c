#include QMK_KEYBOARD_H
#include "f072.h"
#include "oled.c"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT(
		KC_A, KC_B, KC_C, KC_D, 
		KC_E, KC_F, KC_G, KC_H, 
		KC_I, KC_J, KC_K, KC_L, 
		KC_M, KC_N, KC_O, KC_P),


	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	
	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
		
};

/* 编码器
bool encoder_update_user(uint8_t index, bool clockwise) {
if (index == 0) {
if (clockwise) {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 0, 6));
} else {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 1, 6));
}
}if (index == 1) {
if (clockwise) {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 2, 6));
} else {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 3, 6));
}
}if (index == 2) {
if (clockwise) {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 4, 6));
} else {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 5, 6));
}
}if (index == 3) {
if (clockwise) {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 7, 1));
} else {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 7, 2));
}
}if (index == 4) {
if (clockwise) {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 7, 3));
} else {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 7, 4));
}
}if (index == 5) {
if (clockwise) {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 7, 5));
} else {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 7, 6));
}
}if (index == 6) {
if (clockwise) {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 4, 1));
} else {
tap_code16(dynamic_keymap_get_keycode(biton32(layer_state), 4, 2));
}
}
         return true;
} */


/*/// 按下 Capslock 的時候，第6颗RGB之后(也就第7颗) 的4颗灯会亮与第12個灯(也就是第12个) 之后的第4颗会亮紅色。
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, RGB_AZURE}       // Light 4 LEDs, starting with LED 6
);

// Layer 1 启用的時候，第 0颗后1颗灯会亮青色
const rgblight_segment_t PROGMEM my_layer0_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, RGB_AZURE}
);
const rgblight_segment_t PROGMEM my_layer1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, RGB_AZURE}
);
const rgblight_segment_t PROGMEM my_layer2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, RGB_AZURE}
);
const rgblight_segment_t PROGMEM my_layer3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2,1, RGB_AZURE}
);
const rgblight_segment_t PROGMEM my_layer4_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {3,1, RGB_AZURE}
);
const rgblight_segment_t PROGMEM my_layer5_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, RGB_BLUE}
);
const rgblight_segment_t PROGMEM my_layer6_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, RGB_BLUE}
);
const rgblight_segment_t PROGMEM my_layer7_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2,1, RGB_BLUE}
);
const rgblight_segment_t PROGMEM my_layer8_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {3,1, RGB_BLUE}
);
const rgblight_segment_t PROGMEM my_layer9_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, RGB_GREEN}
);
const rgblight_segment_t PROGMEM my_layer10_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, RGB_GREEN}
);
const rgblight_segment_t PROGMEM my_layer11_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2,1, RGB_GREEN}
);
const rgblight_segment_t PROGMEM my_layer12_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {3,1, RGB_GREEN}
);
const rgblight_segment_t PROGMEM my_layer13_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, RGB_WHITE}
);
const rgblight_segment_t PROGMEM my_layer14_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, RGB_WHITE}
);
const rgblight_segment_t PROGMEM my_layer15_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2,1, RGB_WHITE}
);
// etc..



// 接者將您的 rgblight_segment_t 放到 RGBLIGHT_LAYERS_LIST 內
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
//    my_capslock_layer,   // Overrides caps lock layer
    my_layer0_layer, // Overrides other layer 
    my_layer1_layer, // Overrides other layer 
    my_layer2_layer, 
    my_layer3_layer, 
    my_layer4_layer , 
    my_layer5_layer , 
    my_layer6_layer , 
    my_layer7_layer , 
    my_layer8_layer, 
    my_layer9_layer , 
    my_layer10_layer , 
    my_layer11_layer , 
    my_layer12_layer, 
    my_layer13_layer, 
    my_layer14_layer , 
    my_layer15_layer      
);

void keyboard_post_init_user(void) {
    // 启用LED层指示
    rgblight_layers = my_rgb_layers;
}
layer_state_t layer_state_set_user(layer_state_t state) {
    // 如果两个 kb 层都处于活动状态，则两个层都会亮起
    rgblight_set_layer_state(0, layer_state_cmp(state, 0));
    rgblight_set_layer_state(1, layer_state_cmp(state, 1));
    rgblight_set_layer_state(2, layer_state_cmp(state, 2));
    rgblight_set_layer_state(3, layer_state_cmp(state, 3));
    rgblight_set_layer_state(4, layer_state_cmp(state, 4));
    rgblight_set_layer_state(5, layer_state_cmp(state, 5));
    rgblight_set_layer_state(6, layer_state_cmp(state, 6));
    rgblight_set_layer_state(7, layer_state_cmp(state, 7));
    rgblight_set_layer_state(8, layer_state_cmp(state, 8));
    rgblight_set_layer_state(9, layer_state_cmp(state, 9));
    rgblight_set_layer_state(10, layer_state_cmp(state, 10));
    rgblight_set_layer_state(11, layer_state_cmp(state, 11));
    rgblight_set_layer_state(12, layer_state_cmp(state, 12));
    rgblight_set_layer_state(13, layer_state_cmp(state, 13));
    rgblight_set_layer_state(14, layer_state_cmp(state, 14));
    rgblight_set_layer_state(15, layer_state_cmp(state, 15));
    return state;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
} */
