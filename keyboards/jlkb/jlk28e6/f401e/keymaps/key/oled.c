#ifdef OLED_ENABLE
    oled_rotation_t oled_init_user(oled_rotation_t rotation) {
        return OLED_ROTATION_180; 
    }

    // 追踪上一次显示的层
    uint8_t jloled_last_layer = 255; // 初始值设为一个不可能的层索引

    bool oled_task_user(void) {
        // 1. 获取当前层和检查层是否变化
        uint8_t current_layer = get_highest_layer(layer_state);
        bool layer_changed = (current_layer != jloled_last_layer);
        
        // 2. 判断是否需要更新显示
        // 如果层刚刚切换 OR Raw HID 不活跃
        if (layer_changed || !jloled_realtime_active) {
            
            // 无论 jloled_realtime_active 是否为 true，如果层变了就强制显示
            switch (current_layer) {
                case 0:
                    jloled_display_slot(0); // 这一步会清除 jloled_realtime_active
                    break;
                case 1:
                    jloled_display_slot(1);
                    break;
                case 2:
                    jloled_display_slot(2);
                    break;
                case 3:
                    jloled_display_slot(3);
                    break;
                case 4:
                    jloled_display_slot(4);
                    break ;
                case 5:
                    jloled_display_slot(5);
                    break ;
                case 6:
                    jloled_display_slot(6);
                    break ;
                case 7:
                    jloled_display_slot(7);
                    break ;
                case 8:
                    jloled_display_slot(8);
                    break ;
                case 9:
                    jloled_display_slot(9);
                    break ;
                case 10:
                    jloled_display_slot(10);
                    break ;
                case 11:
                    jloled_display_slot(11);
                    break ;
                case 12:
                    jloled_display_slot(12);
                    break ;
                case 13:
                    jloled_display_slot(13);
                    break ;
                case 14:
                    jloled_display_slot(14);
                    break ;
                case 15:
                    jloled_display_slot(15);
                    break ;
                default:
                    // 可选：处理未映射的层
                    jloled_display_slot(0); 
                    break;
            }
            
            // 更新追踪的层状态
            jloled_last_layer = current_layer; 
        }
        
        // 3. 刷新 OLED 
        // jloled_task() 会执行超时检查或将 buffer 内容写入 OLED
        jloled_task();

        return false;
    }
#endif