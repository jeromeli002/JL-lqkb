#include "jloled.h"
#include "raw_hid.h"
#include "string.h"

// 本地显示缓冲区
uint8_t jloled_buffer[JLOLED_BUFFER_SIZE];

// 标记是否需要刷新屏幕
bool jloled_dirty = true;

/**
 * @brief 初始化OLED显示内容
 * 默认显示竖条纹，验证屏幕是否工作
 */
void jloled_init(void) {
    // 初始化为竖条纹 (Vertical Stripes)
    // 0x55 = 01010101 (二进制), 如果按列扫描，表现为细密的竖线
    // 或者使用 0xFF, 0x00 交替来实现更宽的竖条
    for (int i = 0; i < JLOLED_BUFFER_SIZE; i++) {
        // 这里使用 10101010 模式，配合OLED的Vertical addressing
        // 在屏幕上看起来像是细密的网格或竖条，取决于OLED映射方式
        jloled_buffer[i] = 0xAA; 
    }
    jloled_dirty = true;
}

/**
 * @brief OLED 任务函数，放入 oled_task_user 中调用
 * 负责将缓冲区的数据写入 OLED
 */
void jloled_task(void) {
    if (jloled_dirty) {
        // 将缓冲区内容写入 OLED
        // 修正点：增加 (const char *) 强制类型转换，解决 "pointer targets differ in signedness" 报错
        oled_write_raw((const char *)jloled_buffer, JLOLED_BUFFER_SIZE);
        jloled_dirty = false;
    }
}

/**
 * @brief 处理 Raw HID 接收到的数据
 * 放入 raw_hid_receive 中调用
 * * 协议格式 (32 bytes):
 * [0]: 0xAC (魔数/命令字)
 * [1]: Packet Index (包索引/偏移量块号)
 * [2..31]: 30 bytes 图像数据
 */
void jloled_receive(uint8_t *data, uint8_t length) {
    // 1. 检查长度和魔数
    if (length != RAWHID_PACKET_SIZE || data[0] != 0xAC) {
        return;
    }

    // 2. 获取块索引
    uint8_t block_index = data[1];

    // 3. 计算在缓冲区中的目标偏移量
    // 每个包携带 PAYLOAD_SIZE (30) 字节
    uint16_t buffer_offset = block_index * PAYLOAD_SIZE;

    // 4. 安全检查，防止溢出
    if (buffer_offset >= JLOLED_BUFFER_SIZE) {
        return;
    }

    // 5. 计算本次需要写入的字节数 (处理最后一个包可能不满的情况)
    uint16_t bytes_to_copy = PAYLOAD_SIZE;
    if (buffer_offset + bytes_to_copy > JLOLED_BUFFER_SIZE) {
        bytes_to_copy = JLOLED_BUFFER_SIZE - buffer_offset;
    }

    // 6. 复制数据到缓冲区
    // data+2 是因为前两个字节是头信息
    memcpy(&jloled_buffer[buffer_offset], &data[2], bytes_to_copy);

    // 7. 标记为脏，以便下一帧刷新
    jloled_dirty = true;
}