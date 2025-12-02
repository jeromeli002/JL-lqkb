#include QMK_KEYBOARD_H
#include "tm1640.h" // 包含你的驱动头文件
#include "tm1640.c"   // 处理接收到的原始数据
#include "custom_matrix_on.c"   // 处理接收到的原始数据

enum custom_keycodes {
    QK_1 = QK_KB_0,  // 定义自定义键码，用于测试点亮灯
    QK_2,
    test,
    QK_3                   // 定义自定义键码，用于测试熄灭所有灯
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    LAYOUT(
        QK_1, QK_2, test),

    LAYOUT(
        QK_1, QK_1, QK_1)

};

// 按键事件处理函数
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        
        // 检查自定义键码
        switch (keycode) {
            case QK_1:
                tm1640_start_running_light(); 
                return false; // 返回 false，表示这个键不传递给固件的默认处理
            
            case QK_2:
                tm1640_start_running_light(); 
                return false; // 返回 false，表示这个键不传递给固件的默认处理
                
            case test:
                g_remote_rgb_data.h = 214; 
				g_remote_rgb_data.s = 0xFF; 
				g_remote_rgb_data.v = 0xFF; 
				g_remote_rgb_data.spd = 0xFF; 
				g_remote_rgb_data.led_bitmap[0] = 0xA0;
				rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
                return false; // 返回 false，表示这个键不传递给固件的默认处理
                
            case QK_3:
                 tm1640_start_running_light(); 
                return false; // 返回 false，表示这个键不传递给固件的默认处理
                
                
            
        }

    } else {
        // 当按键释放时，可以根据需要恢复下雨效果或显示其他默认图案
        // custom_matrix_light_start_rain(); // 如果希望按键释放后自动恢复
    }
    return true; // 继续处理按键
}
