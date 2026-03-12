// remote_static_color_effect.inc

/**
 * @brief 远程控制的静态纯色模式 (带 LED 选中位图)
 */
static bool remote_static_color(effect_params_t* params) {
    
    // 1. 获取目标颜色 (H S V)
    HSV hsv = {
        .h = g_remote_rgb_data.h,
        .s = g_remote_rgb_data.s,
        .v = g_remote_rgb_data.v
    };
    // 转换为 RGB
    RGB rgb = hsv_to_rgb(hsv);

    // QMK 标准宏，用于获取当前效果需要处理的 LED 范围
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        
        if (i >= MAX_SUPPORTED_LEDS) {
            continue; 
        }
        
        // 计算 LED 索引 i 在位图中的位置
        const uint8_t byte_index = i / 8;
        const uint8_t bit_index = i % 8; 
        
        // MSB 优先的位检查逻辑：(1 << 7) 是最高位
        const uint8_t mask = (1 << (7 - bit_index));

        // 检查位图是否选中当前 LED
        if (g_remote_rgb_data.led_bitmap[byte_index] & mask) {
            // 选中：应用远程颜色
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        // 未选中：不操作，保持原样
    }

    return rgb_matrix_check_finished_leds(led_max);
}