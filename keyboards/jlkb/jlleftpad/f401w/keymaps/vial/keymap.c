#include QMK_KEYBOARD_H
#include "config.h"

#if defined(RGBLIGHT_WS2812)
#    include "ws2812.h"
#endif

#include "bhq_common.h"
#include "wireless.h"
#include "transport.h"
#include "ws2812.h"
#include "color.h"
#include "report_buffer.h"

#include "custom_matrix_on.c"   // 处理接收到的原始数据

enum keycodes {
  LAYERS_DOWN = QK_KB_0,
  LAYERS_UP,
  jltb,
  jld6u7
};

led_t kb_led_state = {0};
uint8_t bat_low_flag = 0;

#define HIGHEST_LAYER 15 //最高层数 0开始算起默认15(16层)
static uint8_t current_layer = 0; //默认0层开始

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT(
		KC_KP_MINUS, KC_KP_ASTERISK, KC_KP_SLASH, KC_NUM_LOCK, KC_TRNS, 
		KC_KP_PLUS, KC_KP_9, KC_KP_8, KC_KP_7, KC_TRNS, 
		KC_TRNS, KC_KP_6, KC_KP_5, KC_KP_4, KC_TRNS, 
		KC_KP_ENTER, KC_KP_3, KC_KP_2, KC_KP_1, KC_TRNS, 
		KC_TRNS, KC_KP_DOT, KC_TRNS, KC_KP_0, KC_TRNS),

	LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] =   {  ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },
    [1] =   {  ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },

    //                  旋钮 1                                          旋钮 2                               
};
#endif

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

// 2812 电源开关
void ws2812_set_power(uint8_t on)
{
    gpio_set_pin_output(WS2812_POWER_PIN);        // ws2812 power
    if(on)  // 开
    {
#if WS2812_POWER_ON_LEVEL == 0
        gpio_write_pin_low(WS2812_POWER_PIN);
#else
        gpio_write_pin_high(WS2812_POWER_PIN);
#endif
    }
    else    // 关
    {
#if WS2812_POWER_ON_LEVEL == 0
        gpio_write_pin_high(WS2812_POWER_PIN);
#else
        gpio_write_pin_low(WS2812_POWER_PIN);
#endif
    }
}

#   if defined(KB_LPM_ENABLED)
// 低功耗外围设备电源控制
void lpm_device_power_open(void) 
{
    // ws2812电源开启
    ws2812_init();
    ws2812_set_power(1);

}
//关闭外围设备电源
void lpm_device_power_close(void) 
{
    // ws2812电源关闭
    rgblight_setrgb_at(0, 0, 0, 0);
    ws2812_set_power(0);
    gpio_set_pin_output(WS2812_DI_PIN);        // ws2812 DI Pin
    gpio_write_pin_low(WS2812_DI_PIN);
}




// 将未使用的引脚设置为输入模拟 
// PS：在6095中，如果不加以下代码休眠时是102ua。如果加了就是30ua~32ua浮动
void lpm_set_unused_pins_to_input_analog(void)
{
    // 禁用调试功能以降低功耗
    DBGMCU->CR &= ~DBGMCU_CR_DBG_SLEEP;   // 禁用在Sleep模式下的调试
    DBGMCU->CR &= ~DBGMCU_CR_DBG_STOP;    // 禁用在Stop模式下的调试
    DBGMCU->CR &= ~DBGMCU_CR_DBG_STANDBY; // 禁用在Standby模式下的调试
    // 在系统初始化代码中禁用SWD接口
    // palSetLineMode(A13, PAL_MODE_INPUT_ANALOG);
    // palSetLineMode(A14, PAL_MODE_INPUT_ANALOG);

    // palSetLineMode(A0, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A1, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A2, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A3, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A4, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A5, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A6, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A7, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A8, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A9, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A10, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A11, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A13, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A14, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(A15, PAL_MODE_INPUT_ANALOG); 

    // palSetLineMode(B0, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B1, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B2, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B3, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B4, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B5, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B6, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B7, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B8, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B9, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B10, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B11, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B13, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B14, PAL_MODE_INPUT_ANALOG); 
    // palSetLineMode(B15, PAL_MODE_INPUT_ANALOG); 
}

#endif
