#include "jloled.h"
#include "raw_hid.h"
#include "string.h"
#include "eeprom.h"
#include "eeconfig.h"
#include "timer.h" // QMK 定时器头文件

// --- 全局变量定义 ---
// 本地显示缓冲区
uint8_t jloled_buffer[JLOLED_BUFFER_SIZE];
bool jloled_dirty = true;

// EEPROM 存储起始地址变量的定义
uint16_t jloled_eeprom_start_addr = 2048;

// 新增：实时显示状态标志位
bool jloled_realtime_active = false; 

// 新增：实时显示定时器和超时设置 (单位：毫秒)
uint32_t jloled_realtime_timer = 0;

/**
 * @brief 将数据包写入 RAM 缓冲区 (实时显示)
 * @param data Raw HID 数据
 * @param length 数据长度
 */
static void jloled_write_ram(uint8_t *data, uint8_t length) {
    if (length != RAWHID_PACKET_SIZE) return;

    uint8_t block_index = data[1];
    uint16_t buffer_offset = block_index * PAYLOAD_SIZE;

    if (buffer_offset >= JLOLED_BUFFER_SIZE) return;

    uint16_t bytes_to_copy = PAYLOAD_SIZE;
    if (buffer_offset + bytes_to_copy > JLOLED_BUFFER_SIZE) {
        bytes_to_copy = JLOLED_BUFFER_SIZE - buffer_offset;
    }

    // 写入 RAM 缓冲区
    memcpy(jloled_buffer + buffer_offset, &data[2], bytes_to_copy);
    jloled_dirty = true;
    
    // 关键修改：激活实时显示标志并重置计时器
    jloled_realtime_active = true;
    jloled_realtime_timer = timer_read32();
}

/**
 * @brief 将数据包写入 EEPROM 槽位
 * @param data Raw HID 数据
 * @param length 数据长度
 * @param slot_index 目标槽位索引
 */
static void jloled_write_eeprom(uint8_t *data, uint8_t length, uint8_t slot_index) {
    if (length != RAWHID_PACKET_SIZE) return;

    uint8_t block_index = data[1];
    uint16_t buffer_offset = block_index * PAYLOAD_SIZE;

    if (buffer_offset >= JLOLED_SLOT_SIZE) return;

    uint16_t bytes_to_copy = PAYLOAD_SIZE;
    if (buffer_offset + bytes_to_copy > JLOLED_SLOT_SIZE) {
        bytes_to_copy = JLOLED_SLOT_SIZE - buffer_offset;
    }

    // 计算 EEPROM 目标偏移量
    uint16_t eeprom_base_offset = jloled_eeprom_start_addr + (slot_index * JLOLED_SLOT_SIZE);
    uint16_t eeprom_dest_offset = eeprom_base_offset + buffer_offset;
    
    // 写入 EEPROM
    eeprom_update_block((const void *)&data[2], (void *)(uintptr_t)eeprom_dest_offset, bytes_to_copy);
}


/**
 * @brief 处理 Raw HID 接收到的数据
 */
void jloled_receive(uint8_t *data, uint8_t length) {
    uint8_t magic = data[0];

    // --- 1. 实时显示 (写入 RAM) ---
    if (magic == JLOLED_MAGIC_REALTIME) { // 0xAC
        jloled_write_ram(data, length);
        return;
    }

    // --- 2. 写入 EEPROM 槽位 ---
    if (magic >= JLOLED_MAGIC_WRITE_EEPROM_BASE && magic < (JLOLED_MAGIC_WRITE_EEPROM_BASE + JLOLED_SLOT_COUNT)) {
        uint8_t slot_index = magic - JLOLED_MAGIC_WRITE_EEPROM_BASE;
        
        // 假设 EEPROM 足够大
        
        jloled_write_eeprom(data, length, slot_index);
        return;
    }

    // --- 3. 调用 EEPROM 槽位显示 ---
    if (magic == JLOLED_MAGIC_DISPLAY_SLOT) { 
        uint8_t slot_index = data[1];

        if (slot_index < JLOLED_SLOT_COUNT) {
            jloled_display_slot(slot_index);
            jloled_realtime_active = true;
            jloled_realtime_timer = timer_read32();
        }
        return;
    }
}

/**
 * @brief OLED 任务函数，放入 oled_task_user 中调用
 */
void jloled_task(void) {
    // 关键修改：实时显示超时检查
   if (jloled_realtime_active && timer_elapsed32(jloled_realtime_timer) > JLOLED_REALTIME_TIMEOUT) {
        jloled_realtime_active = false; // 超时：关闭实时显示标志
        jloled_dirty = true;            
        return;                         
    }
    
    if (jloled_dirty) {
        oled_write_raw((const char *)jloled_buffer, JLOLED_BUFFER_SIZE);
        jloled_dirty = false;
    }
}

/**
 * @brief 从 EEPROM 槽位读取图像并显示
 * @param slot_index 要显示的槽位 (0-31)
 */
void jloled_display_slot(uint8_t slot_index) {
    if (slot_index >= JLOLED_SLOT_COUNT) {
        return;
    }

    uint16_t eeprom_src_offset = jloled_eeprom_start_addr + (slot_index * JLOLED_SLOT_SIZE);

    // 修正 eeprom_read_block 的地址类型：强制转换为 const void *
    eeprom_read_block(jloled_buffer, (const void *)(uintptr_t)eeprom_src_offset, JLOLED_SLOT_SIZE);

    jloled_dirty = true;
    
    // 关键修改：当强制显示 EEPROM 槽位时，关闭实时显示标志
    jloled_realtime_active = false;
}