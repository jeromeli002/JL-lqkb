// 接收数据处理
// 适用于 MATRIX_LIGHT_COLS > 8 的通用版本
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    // 仅在数据头为 0xAB 时才继续处理，减少无效判断
    if (data[0] == 0xAC) {
        jloled_receive(data, length);
        return; // 处理完图像就直接返回，不继续执行后面的逻辑
    }
    
    if (data[0] != 0xAB) return;

    // 根据命令位分支处理（仅 A1 需要长度校验，其他命令无需数据体）
    switch (data[1]) {

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


        case 0xA7: // 高阻抗
            gpio_set_pin_input(A14);
            //gpio_write_pin_high(A14);
            break;
        
        case 0xA8: // 引脚置低
            gpio_set_pin_output(A14);
            gpio_write_pin_low(A14);
            break;

        // 其他命令默认不处理
        default:
            break;
    }
}