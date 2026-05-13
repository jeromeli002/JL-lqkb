#include QMK_KEYBOARD_H
#include "tm1640.h"            // 驱动头文件
#include "tm1640.c"            // 驱动实现
#include "custom_matrix_on.c"  // 矩阵逻辑实现

enum custom_keycodes {
    QK_1 = QK_KB_0,  // 定义自定义键码
    QK_2,
    test,
    QK_3
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        QK_1, QK_2, test
    ),
    [1] = LAYOUT(
        QK_1, QK_1, QK_1
    )
};

// 按键事件处理函数
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // --- 仅在按键按下时执行的内容 ---
    if (record->event.pressed) {
        
        // 1. RGB自动休眠逻辑：更新计时并唤醒
        custom_last_activity_time = timer_read32(); 
        if (is_rgb_timeout_sleep) {
            is_rgb_timeout_sleep = false;
            rgb_matrix_enable_noeeprom(); 
        }
        //按键蜂鸣器
        setPinOutput(C1);
        writePinLow(C1);                  // 拉低 -> 导通 MOS -> 蜂鸣器发声
        wait_ms(100);                       // 持续 5ms
        setPinInput(C1);
        

        // 2. 自定义键码分支处理
        switch (keycode) {
            case QK_1:
            case QK_2:
            case QK_3:
                tm1640_start_running_light(); 
                return false; // 拦截此键，不发送给系统

            case test:
                g_remote_rgb_data.h = 214; 
                g_remote_rgb_data.s = 0xFF; 
                g_remote_rgb_data.v = 0xFF; 
                g_remote_rgb_data.spd = 0xFF; 
                g_remote_rgb_data.led_bitmap[0] = 0xA0;
                rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
                return false; // 拦截此键
        }
    } 
    // --- 按键释放（松开）或其他情况 ---
    else {
        // 如果以后有松开按键时的逻辑，写在这里
    }

    // 默认返回 true，确保非自定义按键（如普通字母、松开动作）能正常工作
    return true; 
}