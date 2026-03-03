// remote_static_mode0_effect.inc

/**
 * @brief 自定义：模式 0 - 根据 LED 索引设置不同颜色
 * 示例硬编码效果
 */
static bool remote_static_mode0(effect_params_t* params) {
    
    // QMK 标准宏，用于获取当前效果需要处理的 LED 范围
    RGB_MATRIX_USE_LIMITS(led_min, led_max); 
    
    for (uint8_t i = led_min; i < led_max; i++) {
        // ----------------------------------------------------
        // 核心逻辑：根据 LED 索引 i 来设置颜色
        // ----------------------------------------------------
        if (i == 0) {
            // LED 索引 0 设置为 红色
            rgb_matrix_set_color(i, 255, 0, 0); 
        } else if (i == 1) {
            // LED 索引 1 设置为 绿色
            rgb_matrix_set_color(i, 0, 255, 0); 
        } else if (i >= 2 && i <= 5) {
            // LED 索引 2 到 5 设置为 蓝色
            rgb_matrix_set_color(i, 0, 0, 255); 
        } else {
            // 其他 LED 设置为 黑色 (关闭)
            rgb_matrix_set_color(i, 0, 0, 0); 
        }
    }

    // 告诉 RGB Matrix 模块是否处理完毕
    return rgb_matrix_check_finished_leds(led_max);
}