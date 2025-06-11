#include QMK_KEYBOARD_H

enum keycodes {
  LAYERS_DOWN = QK_KB_0,
  LAYERS_UP,
  jltb,
  jld6u7
};

#define HIGHEST_LAYER 4 //最高层数 0开始算起默认15(16层)
static uint8_t current_layer = 0; //默认0层开始

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT(
KC_ESCAPE, KC_F2, KC_F4, KC_F6, KC_F8, KC_F10, KC_F12, KC_SCRL, KC_NUM, KC_KP_ASTERISK, KC_MUTE,
KC_F1, KC_F3, KC_F5, KC_F7, KC_F9, KC_F11, KC_PSCR, KC_PAUSE, KC_KP_SLASH, KC_KP_MINUS, DIAL_BUT, 
KC_GRAVE, KC_2, KC_4, KC_6, KC_8, KC_0, KC_EQUAL, KC_INSERT, KC_PGUP, KC_KP_9, KC_MPLY, 
KC_1, KC_3, KC_5, KC_7, KC_9, KC_MINUS, KC_BSPC, KC_HOME, KC_KP_8, KC_KP_4, KC_TRNS, 
KC_TAB, KC_W, KC_R, KC_Y, KC_I, KC_P, KC_RBRC, KC_DELETE, KC_PGDN, KC_KP_5, KC_TRNS, 
KC_Q, KC_E, KC_T, KC_U, KC_O, KC_LBRC, KC_BSLS, KC_END, KC_KP_7, KC_KP_6, KC_TRNS, 
KC_CAPS, KC_S, KC_F, KC_H, KC_K, KC_SCLN, KC_ENTER, KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_PLUS, 
KC_A, KC_D, KC_G, KC_J, KC_L, KC_QUOTE, KC_UP, KC_RIGHT, KC_KP_0, KC_KP_DOT, KC_KP_ENTER, 
KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_N, KC_COMMA, KC_SLASH, KC_RSFT, KC_LEFT, KC_DOWN, 
KC_LCTL, KC_LGUI, KC_LALT, KC_SPACE, KC_B, KC_M, KC_DOT, KC_RALT, KC_RGUI, KC_APPLICATION, KC_RCTL),

	LAYOUT(
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) , ENCODER_CCW_CW(KC_MPRV, KC_MNXT) , ENCODER_CCW_CW(DIAL_L, DIAL_R)  },
    [1] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) , ENCODER_CCW_CW(KC_MPRV, KC_MNXT) , ENCODER_CCW_CW(DIAL_L, DIAL_R)  },

    //                  旋钮 1                                          旋钮 2                                              旋钮 3              
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

    // 打开淘宝店
    case jltb:
        if (record->event.pressed) {
            SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_R) SS_UP(X_LGUI) SS_DELAY(100) "https://jlkb.taobao.com" SS_TAP(X_ENTER) SS_TAP(X_ENTER));
        }
     return false;
     
      //按下6层抬起7层    
     case jld6u7:
        if (record->event.pressed) {
           tap_code16(keymap_key_to_keycode(6, record->event.key));
        }else {
           tap_code16(keymap_key_to_keycode(7, record->event.key)); 
      }
    return false;

   // 下一个自定义键
    default:
      return true;
  }
}

/*/// 按下 Capslock 的時候，第6颗RGB之后(也就第7颗) 的4颗灯会亮与第12個灯(也就是第12个) 之后的第4颗会亮紅色。
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, RGB_AZURE}       // Light 4 LEDs, starting with LED 6
);
 */
// Layer 1 启用的時候，{1,2, RGB_WHITE}第 1颗开始2颗灯会亮白色
const rgblight_segment_t PROGMEM my_layer0_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, HSV_AZURE}
);
const rgblight_segment_t PROGMEM my_layer1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {5,1, HSV_AZURE}
);
const rgblight_segment_t PROGMEM my_layer2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {4,1, HSV_AZURE}
);
const rgblight_segment_t PROGMEM my_layer3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {3,1, HSV_AZURE}
);
// etc..

// 接着將您的 rgblight_segment_t 放到 RGBLIGHT_LAYERS_LIST 內
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
//    my_capslock_layer,   // Overrides caps lock layer
    my_layer0_layer, // Overrides other layer 
    my_layer1_layer, // Overrides other layer 
    my_layer2_layer, 
    my_layer3_layer     
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
    return state;
}