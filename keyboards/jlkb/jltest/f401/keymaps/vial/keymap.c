#include QMK_KEYBOARD_H
#include "led_matrix_light.h" // 包含你的驱动头文件
#include "led_matrix_on.c"   // 处理接收到的原始数据

void keyboard_post_init_user(void) {
    custom_matrix_light_init();
    
    // **调用效果1：逐颗点亮** (作为初始化效果)
    custom_matrix_light_start_effect(MATRIX_EFFECT_SCAN_INIT); 
    
    // 如果想使用效果2：(闪烁3次)
   //custom_matrix_light_start_effect(MATRIX_EFFECT_FLASH_INIT);
}

enum custom_keycodes {
    QK_LED_ON = QK_KB_0,  // 定义自定义键码，用于测试点亮灯
    QK_OUT1,
    QK_OUT2,
    QK_OUT3,
    QK_LED_ON2,
    ML_OFF                   // 定义自定义键码，用于测试熄灭所有灯
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    LAYOUT(
        QK_OUT1, QK_OUT2, QK_BOOT,
        QK_OUT1, QK_OUT3, ML_OFF),

    LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS)

};

// 键盘初始化函数
void keyboard_post_init(void) {
    custom_matrix_light_init(); // 初始化矩阵灯驱动
}
// 主循环任务函数，QMK会不断调用这个函数
void matrix_scan_user(void) {
    custom_matrix_light_task(); // 调用驱动的任务函数来刷新显示和处理动画
}

uint8_t data_out1[32] = {
    0x00, 0x11, 0x33, 0x44, 0x55, 0x00, 0x00, 0x00, 
    0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t data_out2[32] = {
    0x00, 0x22, 0x33, 0x44, 0x55, 0x00, 0x00, 0x00, 
    0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t data_out3[32] = {
    0x00, 0x33, 0x33, 0x44, 0x55, 0x00, 0x00, 0x00, 
    0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 按键事件处理函数
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        
        // 检查自定义键码
        switch (keycode) {
            case QK_LED_ON:
                // 示例：点亮 (1,2) 和 (3,4) 处的灯
                // 现在 custom_matrix_light_set_pixels 不再自动清空矩阵
                // 如果你希望每次点亮时都只显示这些灯，需要先清空自定义像素层
                static const matrix_pixel_t points_to_light[] = {{1, 3}, {1, 5}};
                custom_matrix_light_set_pixels(points_to_light, ARRAY_SIZE(points_to_light), true);
                return false; // 返回 false，表示这个键不传递给固件的默认处理
            
            case QK_OUT1:
                host_raw_hid_send(data_out1, sizeof(data_out1));
                return false; // 返回 false，表示这个键不传递给固件的默认处理
                
            case QK_LED_ON2:
                custom_matrix_light_clear_all(); // 调用函数关闭所有灯（包括字符、像素和雨滴）
                static const matrix_pixel_t points_to_light2[] = {{5, 3}, {5, 5}};
                custom_matrix_light_set_pixels(points_to_light2, ARRAY_SIZE(points_to_light2), true);
                return false; // 返回 false，表示这个键不传递给固件的默认处理
                
            case QK_OUT2:
                host_raw_hid_send(data_out2, sizeof(data_out2));
                return false; // 返回 false，表示这个键不传递给固件的默认处理
                
            case QK_OUT3:
                host_raw_hid_send(data_out3, sizeof(data_out3));
                return false; // 返回 false，表示这个键不传递给固件的默认处理
            
            case ML_OFF:
                custom_matrix_light_clear_all(); // 调用函数关闭所有灯（包括字符、像素和雨滴）
                return false; // 返回 false，表示这个键不传递给固件的默认处理
            
        }

    } else {
        // 当按键释放时，可以根据需要恢复下雨效果或显示其他默认图案
        // custom_matrix_light_start_rain(); // 如果希望按键释放后自动恢复
    }
    return true; // 继续处理按键
}