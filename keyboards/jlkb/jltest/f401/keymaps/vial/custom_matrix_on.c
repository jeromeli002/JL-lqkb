// 接收数据处理
// 适用于 MATRIX_LIGHT_COLS > 8 的通用版本
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    // 计算每行所需的字节数（向上取整：(列数 + 7) / 8）
    const uint8_t BYTES_PER_ROW = (MATRIX_LIGHT_COLS + 7) / 8;
    
    // 预期的数据字节数：1 (data[0] 保留) + 1 (data[1] 命令位) + (行数 * 每行字节数)
    const uint16_t EXPECTED_DATA_BYTES = 2 + (uint16_t)MATRIX_LIGHT_ROWS * BYTES_PER_ROW; 
    
    // 1. 检查数据长度是否符合预期
    // 因为 Raw HID 报文长度通常不超过 64 字节，这里用 length 检查。
    if (length >= EXPECTED_DATA_BYTES) {
        
        // 2. 检查命令/标志位 data[1]
        // 0xA1: 静态点阵数据
        if (data[0] == 0xAB && data[1] == 0xA1) {
            
            custom_matrix_light_start_effect(MATRIX_EFFECT_NONE);
            custom_matrix_light_clear_all(); 
            
            static matrix_pixel_t points_to_light[MATRIX_LIGHT_ROWS * MATRIX_LIGHT_COLS];
            uint8_t current_pixel_count = 0;
            
            // 数据从 data[2] 开始
            for (uint8_t row = 0; row < MATRIX_LIGHT_ROWS; row++) {
                
                // 计算当前行数据的起始索引
                // 起始索引 = data[2] + (当前行 * 每行字节数)
                uint16_t data_start_index = 2 + (uint16_t)row * BYTES_PER_ROW;
                
                for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
                    
                    // 1. 确定当前列位于哪个字节 (0-based index)
                    // 例如：col 0-7 在 byte_idx=0，col 8-15 在 byte_idx=1
                    uint8_t byte_idx = col / 8; 
                    
                    // 2. 确定当前列在它所属字节的哪一位 (0-7)
                    uint8_t bit_idx = col % 8; 
                    
                    // 3. 读取对应的字节数据
                    // data_start_index + byte_idx 就是当前 LED 状态所在的字节
                    uint8_t current_byte = data[data_start_index + byte_idx]; 
                    
                    // 4. 检查该字节的第 bit_idx 位是否为 '1'
                    // (1 << bit_idx) 创建掩码
                    if (current_byte & (1 << bit_idx)) {
                        
                        // 找到了一个需要点亮的灯
                        points_to_light[current_pixel_count].row = row;
                        points_to_light[current_pixel_count].col = col;
                        
                        current_pixel_count++;
                    }
                }
            }
            
            // 3. 一次性点亮所有收集到的像素点
            if (current_pixel_count > 0) {
                 custom_matrix_light_set_pixels(points_to_light, current_pixel_count, true);
            }

        } else if (data[0] == 0xAB && data[1] == 0xA0) {
            // 清空所有灯光
            custom_matrix_light_start_effect(MATRIX_EFFECT_NONE);
            custom_matrix_light_clear_all();
            return;
        } else if (data[0] == 0xAB && data[1] == 0xA2) {
            // 重启进入BL模式
            bootloader_jump();
            return;
        } else if (data[0] == 0xAB && data[1] == 0xA3) {
            // 清空eeprom设置
            eeconfig_init();
            wait_ms(500);
            soft_reset_keyboard();
            return;
        } else if (data[0] == 0xAB && data[1] == 0xA4) {
            // 正常重启不进入BL模式
            soft_reset_keyboard();
            return;
        }else if (data[0] == 0xAB && data[1] == 0xA5) {
            // 正常重启不进入BL模式
            custom_matrix_light_start_effect(MATRIX_EFFECT_FLASH_INIT);
            return;
        }else if (data[0] == 0xAB && data[1] == 0xA6) {
            // 正常重启不进入BL模式
            custom_matrix_light_start_effect(MATRIX_EFFECT_SCAN_INIT); 
            return;
        }
    }
}