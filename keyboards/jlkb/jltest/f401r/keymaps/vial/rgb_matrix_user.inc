// rgb_matrix_user.inc

// !!! DO NOT ADD #pragma once !!! //

// Step 1: 声明新的自定义效果
RGB_MATRIX_EFFECT(remote_static_color)
RGB_MATRIX_EFFECT(remote_static_off) //关闭所有

// Step 2: 在 RGB_MATRIX_CUSTOM_EFFECT_IMPLS 块中定义效果
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "quantum.h"
#include "jlrgb.h" // 引入全局变量声明

/**
 * @brief 远程控制的静态纯色模式 (带 LED 选中位图)
 */
static bool remote_static_color(effect_params_t* params) {
    
    // ... H S V 和 RGB 转换不变 ...
    HSV hsv = {
        .h = g_remote_rgb_data.h,
        .s = g_remote_rgb_data.s,
        .v = g_remote_rgb_data.v
    };
    RGB rgb = hsv_to_rgb(hsv);

    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        
        if (i >= MAX_SUPPORTED_LEDS) {
            continue; 
        }
        
        const uint8_t byte_index = i / 8;
        const uint8_t bit_index = i % 8; 
        
        // MSB 优先的位检查逻辑
        const uint8_t mask = (1 << (7 - bit_index));

        if (g_remote_rgb_data.led_bitmap[byte_index] & mask) {
            // 选中：应用远程颜色
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        // 未选中：不操作，保持原样（或者你可以选择在这里设置为黑色）
    }

    return rgb_matrix_check_finished_leds(led_max);
}

/**
 * @brief 远程控制：全关模式
 * 强制将所有 LED 设置为黑色 (关闭)
 */
static bool remote_static_off(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // 简单粗暴：遍历当前范围内的所有 LED 并关闭
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS