/**
 * @brief 初始化OLED显示内容
 */
void jloled_init(void) {
jloled_display_slot(0); //初始化第一槽位图像
}

// 接收数据处理
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    uint8_t magic = data[0];

    // --- 1. OLED 图像数据/命令处理 (0xAC, 0xB0-0xCF, 0xAD) ---
    // 检查是否是 OLED 相关的 Raw HID 命令
    if (magic == JLOLED_MAGIC_REALTIME || 
        (magic >= JLOLED_MAGIC_WRITE_EEPROM_BASE && magic < (JLOLED_MAGIC_WRITE_EEPROM_BASE + JLOLED_SLOT_COUNT)) ||
        magic == JLOLED_MAGIC_DISPLAY_SLOT) {
        
        jloled_receive(data, length);
        return; 
    }
    
    // --- 2. 系统控制命令处理 (0xAB) ---
    // 如果不是 OLED 命令，则检查是否是系统控制命令
    if (magic == 0xAB) ;

    // 根据命令位分支处理
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
            
        case 0xA5: //第2槽位图像）
            jloled_display_slot(1);
            break;
            
        case 0xA6: //第一槽位图像
            setPinOutput(A14);
            writePinLow(A14);
            break;
            
        case 0xA7: //第一槽位图像
            setPinInput(A14);
            break;

        // ... 其他控制命令 ...

        default:
            break;
    }
}