#include QMK_KEYBOARD_H
#include "tm1640.h" // 包含你的驱动头文件
#include "tm1640.c"   // 处理接收到的原始数据
#include "custom_matrix_on.c"   // 处理接收到的原始数据

enum custom_keycodes {
    QK_1 = QK_KB_0,  // 定义自定义键码，用于测试点亮灯
    QK_2,
    QK_3,
    QK_4                   // 定义自定义键码，用于测试熄灭所有灯
};

#define HIGHEST_LAYER 15 //最高层数 0开始算起默认15(16层)
//static uint8_t current_layer = 0; //默认0层开始

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    LAYOUT(
        QK_1, QK_2, QK_3, QK_4),

    LAYOUT(
        QK_1, QK_1, QK_1, QK_4)

};

static bool c8_state = false;
static uint32_t c8_timeout = 0, c7_timeout = 0; // 极简超时时间戳

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case QK_1:
                c8_state = !c8_state;
                c8_state ? (setPinOutput(C8), writePinHigh(C8)) : (setPinInput(C8), setPinInputHigh(C8));
                return false;
            
            case QK_2:
                setPinOutput(C8); writePinHigh(C8);
                c8_timeout = timer_read() + 5000; // 记录超时时间点
                return false;
                
            case QK_3:
                setPinInput(C7);
                return false;
                
            case QK_4:
                setPinOutput(C7); writePinHigh(C7);
                c7_timeout = timer_read() + 2000; // 记录超时时间点
                return false;
        }
    }
    return true;
}

// 主循环超时检测，极简单行判断
void matrix_scan_user(void) {
    if (c8_timeout && timer_read() >= c8_timeout) { setPinInput(C8); c8_timeout = 0; }
    if (c7_timeout && timer_read() >= c7_timeout) { setPinInput(C7); c7_timeout = 0; }
}


/*/// 按下 Capslock 的時候，第6颗RGB之后(也就第7颗) 的4颗灯会亮与第12個灯(也就是第12个) 之后的第4颗会亮紅色。
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, RGB_AZURE}       // Light 4 LEDs, starting with LED 6
);
 */
// Layer 1 启用的時候，{1,2, RGB_WHITE}第 1颗开始2颗灯会亮白色
const rgblight_segment_t PROGMEM my_layer0_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, HSV_AZURE}
);
const rgblight_segment_t PROGMEM my_layer1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, HSV_PINK}
);
const rgblight_segment_t PROGMEM my_layer2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2,1, HSV_WHITE}
);
const rgblight_segment_t PROGMEM my_layer3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, HSV_AZURE}
);
const rgblight_segment_t PROGMEM my_layer4_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, HSV_AZURE}
);
const rgblight_segment_t PROGMEM my_layer5_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2,1, HSV_PINK}
);
const rgblight_segment_t PROGMEM my_layer6_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, HSV_PINK}
);
const rgblight_segment_t PROGMEM my_layer7_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, HSV_PINK}
);
const rgblight_segment_t PROGMEM my_layer8_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2,1, HSV_PINK}
);
const rgblight_segment_t PROGMEM my_layer9_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, HSV_WHITE}
);
const rgblight_segment_t PROGMEM my_layer10_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, HSV_WHITE}
);
const rgblight_segment_t PROGMEM my_layer11_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2,1, HSV_WHITE}
);
const rgblight_segment_t PROGMEM my_layer12_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, HSV_WHITE}
);
const rgblight_segment_t PROGMEM my_layer13_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,1, HSV_WHITE}
);
const rgblight_segment_t PROGMEM my_layer14_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2,1, HSV_WHITE}
);
const rgblight_segment_t PROGMEM my_layer15_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, HSV_WHITE}
);
// etc..

// 接着將您的 rgblight_segment_t 放到 RGBLIGHT_LAYERS_LIST 內
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
//    my_capslock_layer,   // Overrides caps lock layer
    my_layer0_layer, // Overrides other layer 
    my_layer1_layer, // Overrides other layer 
    my_layer2_layer, 
    my_layer3_layer, 
    my_layer4_layer, 
    my_layer5_layer, 
    my_layer6_layer, 
    my_layer7_layer, 
    my_layer8_layer, 
    my_layer9_layer, 
    my_layer10_layer, 
    my_layer11_layer, 
    my_layer12_layer, 
    my_layer13_layer, 
    my_layer14_layer, 
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