#include "quantum.h"
#include "tm1640.h" // 必须包含 TM1640 头文件
#include <string.h> // 需要用到 memset 或相似的初始化函数

// -------------------------- 宏定义（从 tm1640.h 引用） --------------------------
// 修复编译错误：MATRIX_LIGHT_COLS 和 MATRIX_LIGHT_ROWS 未声明的问题
#define MATRIX_LIGHT_ROWS TM1640_ROWS
#define MATRIX_LIGHT_COLS TM1640_COLS

// -------------------------- 函数声明 --------------------------
// 将函数定义移出 raw_hid_receive_kb，并在此处声明
void handle_external_bitmap_data(const uint8_t *data, uint16_t length, tm1640_brightness_t brightness);

// -------------------------- QMK 初始化函数 --------------------------
void matrix_init_kb(void) {
    tm1640_init();
    // A0 命令的默认效果：闪烁
    tm1640_start_blink(); 
    matrix_init_user();
}

// -------------------------- HID 接收回调函数 --------------------------
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    // 快速过滤：仅处理帧头为 0xAB 的有效报文
    if (data[0] != 0xAB ) return;

    // 根据命令码分支处理
    switch (data[1]) {
        case 0xA0: // 命令：闪烁效果
            tm1640_start_blink();
            break;
            
        case 0xA1: // 命令：精准点阵显示 (调用移至文件作用域的函数)
            // 默认使用最大亮度
            handle_external_bitmap_data(data, length, TM1640_BRIGHTNESS_14_16);
            break;

        case 0xA2: // 命令：重启并进入 Bootloader 模式（用于固件更新）
            bootloader_jump();
            break;

        case 0xA3: // 命令：清空 EEPROM 配置并重启（恢复出厂设置）
            eeconfig_init();
            wait_ms(500); // 延时确保 EEPROM 操作完成
            soft_reset_keyboard();
            break;

        case 0xA4: // 命令：流水灯 
            tm1640_start_running_light(); 
            break;

        case 0xA5: // 闪烁效果（2次）
            tm1640_start_blink();
            break;

        case 0xA6: // 命令：启动流水灯
            tm1640_stop_current_effect();
            tm1640_start_running_light();
            break;
            
        case 0xA7: // 新增命令：关闭所有灯光
            tm1640_stop_current_effect();
            break;
            
        case 0xA8: // 命令：C1 引脚置低
            gpio_set_pin_output(C1);
            gpio_write_pin_low(C1);
            break;

        // 其他未定义命令：默认不处理
        default:
            break;
    }
}

// -------------------------- 外部数据处理函数实现 --------------------------
/**
 * @brief 将外部 Row-Major (行优先) 位图数据转换为 TM1640 Column-Major (列优先) 数据。
 * 适配 TM1640_COLS 配置的列数。
 */
void handle_external_bitmap_data(const uint8_t *data, uint16_t length, tm1640_brightness_t brightness) {
    
    // TM1640 芯片通常限制为 8 行（GRIDs），这里兼容驱动的配置。
    if (MATRIX_LIGHT_ROWS > 8) return; 
    
    // 计算外部数据中表示一行所需的字节数 (向上取整)
    const uint8_t BYTES_PER_ROW = (MATRIX_LIGHT_COLS + 7) / 8;
    
    // 检查报文长度是否足够容纳：帧头(2字节) + 完整的位图数据
    const uint16_t EXPECTED_DATA_BYTES = 2 + (uint16_t)MATRIX_LIGHT_ROWS * BYTES_PER_ROW;

    if (length < EXPECTED_DATA_BYTES) return; 

    // 静态分配 TM1640 所需的列数据缓冲区 (大小由配置决定)
    uint8_t tm1640_col_data[MATRIX_LIGHT_COLS]; 
    memset(tm1640_col_data, 0, MATRIX_LIGHT_COLS); 
    
    // 指向数据体起始位置 data[2]
    const uint8_t *matrix_data = data + 2; 

    // 核心转换逻辑：将行优先的外部数据 (data[row][byte_offset]) 
    // 转换为列优先的 TM1640 数据 (tm1640_col_data[col] 的第 row 位)
    for (uint8_t row = 0; row < MATRIX_LIGHT_ROWS; row++) {
        // 指向当前行数据的起始位置
        const uint8_t *current_row_data = matrix_data + (uint16_t)row * BYTES_PER_ROW;

        for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
            
            // 1. 确定当前列 (col) 在行数据中属于第几个字节 (byte_offset)
            const uint8_t byte_offset = col / 8; 
            
            // 2. 确定当前列 (col) 在该字节中的位索引 (bit_idx)
            const uint8_t bit_idx = col % 8;
            
            // 检查外部数据中的 (row, col) 对应的位是否为 1
            if (current_row_data[byte_offset] & (1 << bit_idx)) {
                
                // 如果是 1，则将 TM1640 输出缓冲区的相应位 (第 'row' 位) 置 1
                // TM1640_col_data[col] 存储了整个第 col 列的数据
                tm1640_col_data[col] |= (1 << row);
            }
        }
    }

    // 调用 TM1640 驱动函数进行显示
    tm1640_display_bitmap(tm1640_col_data, brightness);
}
