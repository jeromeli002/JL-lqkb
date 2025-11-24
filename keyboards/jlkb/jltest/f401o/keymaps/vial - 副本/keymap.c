#include QMK_KEYBOARD_H
// #include "oled.c"
#include "jloled.c"
#include "jloled.h"
#include "custom_matrix_on.c"   // 处理接收到的原始数据

// OLED 任务回调
bool oled_task_user(void) {
    // 调用我们的自定义渲染函数
    jloled_task();
    return false; // 返回 false 表示我们已经处理了绘制，不需要QMK默认绘制
}

// 键盘初始化回调
void keyboard_post_init_user(void) {
    // 初始化 OLED 缓冲区 (竖条纹)
    jloled_init();
}

enum custom_keycodes {
  LAYERS_DOWN = QK_KB_0,
  LAYERS_UP,
  jltb,
  jld6u7
};

#define HIGHEST_LAYER 15 //最高层数 0开始算起默认15(16层)
static uint8_t current_layer = 0; //默认0层开始

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_TRNS, KC_A, KC_TRNS,
		KC_TRNS, KC_B, KC_TRNS),

	[1] = LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS)
};

// 添加新的按键
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // 下一层
  #ifdef OLED_DRIVER_ENABLE
    // 告知 QMK OLED 被“唤醒”了，这会清除 OLED_TIMEOUT 计时器
    oled_on();
#endif
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

