/**
 * @brief Raw HID 数据接收处理函数
 * * 适用于 MATRIX_LIGHT_COLS > 8 的通用版本，且假设 Raw HID 报文长度为固定值（例如 32 字节）。
 * * @param data 接收到的数据指针 (通常为固定长度的报文，例如 32 字节)
 * @param length 接收到的数据长度 (通常固定为 RAW_REPORT_SIZE)
 */
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    
    // 快速失败：仅在数据头为 0xAB 时才继续处理
    if (data[0] != 0xAB) return;

    // 根据命令位分支处理
    switch (data[1]) {
        case 0xA1: { // 静态点阵数据
            // 运行时计算：每行所需的字节数（向上取整：(列数 + 7) / 8）
            const uint8_t BYTES_PER_ROW = (MATRIX_LIGHT_COLS + 7) / 8;
            
            // 预期的数据体字节数
            const uint16_t EXPECTED_DATA_BYTES = 2 + (uint16_t)MATRIX_LIGHT_ROWS * BYTES_PER_ROW;

            // 检查接收到的报文长度是否至少包含完整的矩阵数据体
            // (这是必要的校验，防止数据不完整或越界访问)
            if (length < EXPECTED_DATA_BYTES) return; 

            custom_matrix_light_start_effect(MATRIX_EFFECT_NONE);
            custom_matrix_light_clear_all(); 
            
            // RAM 优化策略：使用局部变量数组，将 RAM 消耗从静态区转移到堆栈区。
            // 警告：如果矩阵过大，可能会导致堆栈溢出。
            matrix_pixel_t points_to_light[MATRIX_LIGHT_ROWS * MATRIX_LIGHT_COLS];
            uint8_t current_pixel_count = 0;
            
            // 性能优化：使用指针直接指向数据体起始位置 data[2]
            const uint8_t *matrix_data = data + 2; 

            // 遍历所有行列，收集需要点亮的像素点
            for (uint8_t row = 0; row < MATRIX_LIGHT_ROWS; row++) {
                // 性能优化：指针指向当前行数据
                const uint8_t *row_data = matrix_data + (uint16_t)row * BYTES_PER_ROW;

                for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
                    const uint8_t byte_idx = col / 8;
                    const uint8_t bit_idx  = col % 8;
                    
                    // 检查对应位是否为 1
                    if (row_data[byte_idx] & (1 << bit_idx)) {
                        // 性能优化：使用复合字面量初始化结构体并递增计数器
                        points_to_light[current_pixel_count++] = (matrix_pixel_t){row, col};
                        
                        // 安全检查：防止收集的像素点数超过数组容量
                        if (current_pixel_count >= (uint8_t)ARRAY_SIZE(points_to_light)) {
                             goto end_of_a1_process;
                        }
                    }
                }
            }
            
        end_of_a1_process:
            // 修复编译错误：调用原始函数，一次性点亮所有收集到的像素点
            if (current_pixel_count > 0) {
                custom_matrix_light_set_pixels(points_to_light, current_pixel_count, true);
            }
            
            break;
        }

        case 0xA0: // 清空所有灯光
            custom_matrix_light_start_effect(MATRIX_EFFECT_NONE);
            custom_matrix_light_clear_all();
            break;

        case 0xA2: // 重启进入BL模式
            bootloader_jump();
            break;

        case 0xA3: { // 清空eeprom设置并重启
            eeconfig_init();
            wait_ms(500); 
            soft_reset_keyboard();
            break;
        }

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