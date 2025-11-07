// 接收数据处理
// 适用于 MATRIX_LIGHT_COLS > 8 的通用版本
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    // 仅在数据头为 0xAB 时才继续处理，减少无效判断
    if (data[0] != 0xAB) return;

    // 计算每行所需的字节数（向上取整：(列数 + 7) / 8）
    const uint8_t BYTES_PER_ROW = (MATRIX_LIGHT_COLS + 7) / 8;
    // 预期的数据字节数：1 (data[0] 保留) + 1 (data[1] 命令位) + (行数 * 每行字节数)
    const uint16_t EXPECTED_DATA_BYTES = 2 + (uint16_t)MATRIX_LIGHT_ROWS * BYTES_PER_ROW; 

    // 根据命令位分支处理（仅 A1 需要长度校验，其他命令无需数据体）
    switch (data[1]) {
        case 0xA1: // 静态点阵数据
            // 检查数据长度是否符合预期（Raw HID 报文长度通常不超过 64 字节）
            if (length >= EXPECTED_DATA_BYTES) {
                custom_matrix_light_start_effect(MATRIX_EFFECT_NONE);
                custom_matrix_light_clear_all(); 
                
                static matrix_pixel_t points_to_light[MATRIX_LIGHT_ROWS * MATRIX_LIGHT_COLS];
                uint8_t current_pixel_count = 0;
                
                // 数据从 data[2] 开始，遍历所有行列
                for (uint8_t row = 0; row < MATRIX_LIGHT_ROWS; row++) {
                    const uint16_t data_start_index = 2 + (uint16_t)row * BYTES_PER_ROW;
                    for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
                        // 计算当前列对应的字节索引和位索引
                        const uint8_t byte_idx = col / 8;
                        const uint8_t bit_idx = col % 8;
                        const uint8_t current_byte = data[data_start_index + byte_idx];
                        
                        // 检查对应位是否为 1，收集需要点亮的像素点
                        if (current_byte & (1 << bit_idx)) {
                            points_to_light[current_pixel_count] = (matrix_pixel_t){row, col};
                            current_pixel_count++;
                        }
                    }
                }
                
                // 一次性点亮所有收集到的像素点
                if (current_pixel_count > 0) {
                    custom_matrix_light_set_pixels(points_to_light, current_pixel_count, true);
                }
            }
            break;

        case 0xA0: // 清空所有灯光
            custom_matrix_light_start_effect(MATRIX_EFFECT_NONE);
            custom_matrix_light_clear_all();
            break;

        case 0xA2: // 重启进入BL模式
            bootloader_jump();
            break;

        case 0xA3: // 清空eeprom设置并重启
            eeconfig_init();
            wait_ms(500);
            soft_reset_keyboard();
            break;

        case 0xA4: // 正常重启（不进入BL模式）
            soft_reset_keyboard();
            break;

        case 0xA5: // 启动闪烁效果
            custom_matrix_light_start_effect(MATRIX_EFFECT_FLASH_INIT);
            break;

        case 0xA6: // 启动扫描效果
            custom_matrix_light_start_effect(MATRIX_EFFECT_SCAN_INIT);
            break;

        case 0xA7: // A14引脚置高
            gpio_set_pin_output(A14);
            gpio_write_pin_high(A14);
            break;
        
        case 0xA8: // A14引脚置低
            gpio_set_pin_output(A14);
            gpio_write_pin_low(A14);
            break;

        // 其他命令默认不处理
        default:
            break;
    }
}