// remote_static_off_effect.inc

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