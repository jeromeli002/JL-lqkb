#include "tm1640.h"
#include "timer.h"
#include "gpio.h"
#include <stdbool.h>
#include <string.h>

// -------------------------- 全局状态变量 --------------------------
static volatile tm1640_effect_type_t g_current_effect = TM1640_EFFECT_NONE;
static uint32_t g_last_update_timer = 0; 
static uint8_t g_blink_step = 0;       
static uint8_t g_running_index = 0;    
static uint8_t g_static_bitmap[TM1640_COLS] = {0}; 

// -------------------------- TM1640 基础通信 (改为 inline 优化固件大小) --------------------------
#define TM1640_DELAY() { __asm__ __volatile__ ("nop\n\t"); }

static inline void tm1640_start(void) {
    writePinHigh(TM1640_SCLK_PIN); TM1640_DELAY();
    writePinHigh(TM1640_DIN_PIN); TM1640_DELAY();
    writePinLow(TM1640_DIN_PIN); TM1640_DELAY();
    writePinLow(TM1640_SCLK_PIN); TM1640_DELAY();
}

static inline void tm1640_stop(void) {
    writePinLow(TM1640_DIN_PIN); TM1640_DELAY();
    writePinHigh(TM1640_SCLK_PIN); TM1640_DELAY();
    writePinHigh(TM1640_DIN_PIN); TM1640_DELAY();
    writePinLow(TM1640_SCLK_PIN); TM1640_DELAY();
}

// 精简 send_byte 逻辑
static inline void tm1640_send_byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        writePinLow(TM1640_SCLK_PIN); TM1640_DELAY();
        writePin(TM1640_DIN_PIN, data & 0x01); // 简化位操作
        TM1640_DELAY();
        writePinHigh(TM1640_SCLK_PIN); TM1640_DELAY();
        data >>= 1;
    }
    writePinLow(TM1640_SCLK_PIN); TM1640_DELAY();
    writePinLow(TM1640_DIN_PIN);
}

void tm1640_display_off(void) {
    tm1640_start();
    tm1640_send_byte(0x80); // DISPLAY_OFF_CMD
    tm1640_stop();
}

// -------------------------- 核心函数：数据发送 --------------------------
void tm1640_send_data(const uint8_t *data, tm1640_brightness_t brightness) {
    if (data == NULL) return;
    
    // 1. 数据命令：自动地址递增 (0x40)
    tm1640_start();
    tm1640_send_byte(0x40);
    tm1640_stop();
    
    // 2. 地址设置：起始地址 (0xC0)
    tm1640_start();
    tm1640_send_byte(0xC0);
    TM1640_DELAY();
    
    // 3. 传输数据
    for (uint8_t i = 0; i < TM1640_COLS; i++) {
        tm1640_send_byte(data[i]);
    }
    tm1640_stop();
    
    // 4. 显示控制命令 (设置亮度/开启显示)
    tm1640_start();
    tm1640_send_byte(brightness); 
    tm1640_stop();
}

// -------------------------- 外部接口（非阻塞式启动） --------------------------
void tm1640_stop_current_effect(void) {
    g_current_effect = TM1640_EFFECT_NONE;
    tm1640_display_off(); 
}

void tm1640_start_blink(void) {
    tm1640_stop_current_effect();
    g_current_effect = TM1640_EFFECT_BLINK;
    g_blink_step = 1; 
    g_last_update_timer = timer_read();
    
    memset(g_static_bitmap, 0xFF, TM1640_COLS);
    tm1640_send_data(g_static_bitmap, TM1640_DEFAULT_BRIGHTNESS_CMD);
}

void tm1640_start_running_light(void) {
    tm1640_stop_current_effect();
    g_current_effect = TM1640_EFFECT_RUNNING_LIGHT;
    
    g_running_index = 0; 
    
    memset(g_static_bitmap, 0, TM1640_COLS);

    // 计算第一个点的 (col, row)
    uint8_t start_col = 0, start_row = 0;
    
    // 两种方向的第一个点都是 (0, 0)，无需复杂计算
    
    // 在启动时立即点亮第一个点
    g_static_bitmap[start_col] |= (1 << start_row);
    tm1640_send_data(g_static_bitmap, TM1640_DEFAULT_BRIGHTNESS_CMD);
    
    g_last_update_timer = timer_read();
}

void tm1640_display_bitmap(const uint8_t *bitmap_data, tm1640_brightness_t brightness) {
    if (bitmap_data == NULL) return;

    tm1640_stop_current_effect(); 
    
    memcpy(g_static_bitmap, bitmap_data, TM1640_COLS);
    tm1640_send_data(g_static_bitmap, brightness);
    
    g_current_effect = TM1640_EFFECT_STATIC;
}

// -------------------------- QMK 任务处理：非阻塞式 (精简逻辑) --------------------------
void tm1640_task(void) {
    const uint8_t TOTAL_PIXELS = TM1640_ROWS * TM1640_COLS;
    uint8_t current_interval = (g_current_effect == TM1640_EFFECT_BLINK) ? TM1640_BLINK_INTERVAL : TM1640_RUNNING_SPEED;

    // 联合判断并使用短路逻辑
    if (g_current_effect == TM1640_EFFECT_NONE || timer_elapsed(g_last_update_timer) < current_interval) {
        return;
    }
    
    g_last_update_timer = timer_read();
    
    switch (g_current_effect) {
        case TM1640_EFFECT_BLINK:
            if (g_blink_step >= TM1640_BLINK_COUNT * 2) {
                tm1640_stop_current_effect();
                break;
            }
            
            g_blink_step++;
            
            // 奇数亮，偶数灭
            if (g_blink_step & 0x01) { // 使用位操作代替 % 2
                tm1640_send_data(g_static_bitmap, TM1640_DEFAULT_BRIGHTNESS_CMD);
            } else { 
                tm1640_display_off();
            }
            break;

        case TM1640_EFFECT_RUNNING_LIGHT:
            
            // 计算当前/下一个点的位置
            uint8_t prev_col, prev_row;
            uint8_t next_col, next_row;
            
            // 熄灭当前点 (g_running_index)
            if (TM1640_RUNNING_DIRECTION == 0) { // 纵向： col/rows, row%rows
                prev_col = g_running_index / TM1640_ROWS;
                prev_row = g_running_index % TM1640_ROWS;
            } else { // 横向： row/cols, col%cols
                prev_row = g_running_index / TM1640_COLS;
                prev_col = g_running_index % TM1640_COLS;
            }
            g_static_bitmap[prev_col] &= ~(1 << prev_row);

            // 推进到下一个点
            g_running_index++;
            
            if (g_running_index >= TOTAL_PIXELS) {
                tm1640_stop_current_effect();
                break;
            }
            
            // 点亮新点 (g_running_index)
            if (TM1640_RUNNING_DIRECTION == 0) { 
                next_col = g_running_index / TM1640_ROWS;
                next_row = g_running_index % TM1640_ROWS;
            } else { 
                next_row = g_running_index / TM1640_COLS;
                next_col = g_running_index % TM1640_COLS;
            }
            g_static_bitmap[next_col] |= (1 << next_row);
            
            tm1640_send_data(g_static_bitmap, TM1640_DEFAULT_BRIGHTNESS_CMD);
            break;
            
        default:
            break;
    }
}


void tm1640_init(void) {
    setPinOutput(TM1640_DIN_PIN);
    setPinOutput(TM1640_SCLK_PIN);
    
    writePinLow(TM1640_SCLK_PIN);
    writePinLow(TM1640_DIN_PIN);
    
    g_current_effect = TM1640_EFFECT_NONE;
    tm1640_display_off();
}