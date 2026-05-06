#include QMK_KEYBOARD_H
#include "custom_matrix_on.c"   // 处理接收到的原始数据

enum keycodes {
  LAYERS_DOWN = QK_KB_0,
  LAYERS_UP,
  jltb,
  jld6u7
};

#define HIGHEST_LAYER 15 //最高层数 0开始算起默认15(16层)
static uint8_t current_layer = 0; //默认0层开始

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT(
KC_VOLD, KC_MUTE, KC_VOLU, KC_CALC, KC_PSCR, KC_SCRL, KC_PAUSE, KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F12,
KC_NUM, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS, KC_INSERT, KC_HOME, KC_PGUP, KC_GRAVE, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_F11, KC_BSPC,
KC_KP_7, KC_KP_8, KC_KP_9, KC_DELETE, KC_END, KC_PGDN, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_0, KC_MINUS, KC_EQUAL,
KC_KP_PLUS, KC_KP_4, KC_KP_5, KC_KP_6, KC_NO, KC_NO, KC_NO, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
KC_KP_1, KC_KP_2, KC_KP_3, KC_UP, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMMA, KC_DOT, KC_K, KC_L, KC_SCLN, KC_QUOTE, KC_ENTER,
KC_KP_ENTER, KC_KP_0, KC_KP_DOT, KC_LEFT, KC_DOWN, KC_RIGHT, KC_LCTL, KC_LGUI, KC_LALT, KC_NO, KC_SPACE, KC_NO, KC_RALT, KC_RGUI, KC_APP, KC_NO, KC_SLASH, KC_RSFT, KC_RCTL),

	LAYOUT(
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) , ENCODER_CCW_CW(KC_MPRV, KC_MNXT) , ENCODER_CCW_CW(DIAL_L, DIAL_R)  },
    [1] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) , ENCODER_CCW_CW(KC_MPRV, KC_MNXT) , ENCODER_CCW_CW(DIAL_L, DIAL_R)  },

    //                  旋钮 1                            旋钮 2                                    旋钮 3              
};
#endif

// 添加新的按键
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
 
 // RGB自动关闭计时
 // 捕获按键事件，用于重置自动休眠计时与唤醒
 if (record->event.pressed) {
        custom_last_activity_time = timer_read32(); // 重置计时
        
        // 如果按键时处于休眠状态且配置非0，则唤醒RGB矩阵
        if (is_rgb_timeout_sleep) {
            is_rgb_timeout_sleep = false;
            rgb_matrix_enable_noeeprom(); 
        }
    }
    return true;
    
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
