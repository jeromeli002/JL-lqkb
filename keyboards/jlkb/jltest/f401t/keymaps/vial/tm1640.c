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

// -------------------------- TM1640 基础通信 --------------------------
#define TM1640_DELAY() { __asm__ __volatile__ ("nop\n\t"); }

static void tm1640_start(void) {
    writePinHigh(TM1640_SCLK_PIN); TM1640_DELAY();
    writePinHigh(TM1640_DIN_PIN); TM1640_DELAY();
    writePinLow(TM1640_DIN_PIN); TM1640_DELAY();
    writePinLow(TM1640_SCLK_PIN); TM1640_DELAY();
}

static void tm1640_stop(void) {
    writePinLow(TM1640_DIN_PIN); TM1640_DELAY();
    writePinHigh(TM1640_SCLK_PIN); TM1640_DELAY();
    writePinHigh(TM1640_DIN_PIN); TM1640_DELAY();
    writePinLow(TM1640_SCLK_PIN); TM1640_DELAY();
}

static void tm1640_send_byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        writePinLow(TM1640_SCLK_PIN); TM1640_DELAY();
        (data & 0x01) ? writePinHigh(TM1640_DIN_PIN) : writePinLow(TM1640_DIN_PIN);
        TM1640_DELAY();
        writePinHigh(TM1640_SCLK_PIN); TM1640_DELAY();
        data >>= 1;
    }
    writePinLow(TM1640_SCLK_PIN); TM1640_DELAY();
    writePinLow(TM1640_DIN_PIN);
}

void tm1640_display_off(void) {
    tm1640_start();
    tm1640_send_byte(0x80); 
    tm1640_stop();
}

// -------------------------- 核心函数：数据发送 --------------------------
// brightness 现在是一个 0x88 到 0x8F 的 TM1640 命令
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
    tm1640_send_byte(brightness); // 使用传入的亮度命令
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
    // 【修改】使用配置的默认亮度
    tm1640_send_data(g_static_bitmap, TM1640_DEFAULT_BRIGHTNESS_CMD);
}

void tm1640_start_running_light(void) {
    tm1640_stop_current_effect();
    g_current_effect = TM1640_EFFECT_RUNNING_LIGHT;
    
    g_running_index = 0; 
    
    memset(g_static_bitmap, 0, TM1640_COLS);

    // 计算第一个点的 (col, row)
    uint8_t start_col, start_row;
    if (TM1640_RUNNING_DIRECTION == 0) { // 纵向：先列后行 (col = index / ROWS, row = index % ROWS)
        start_col = 0;
        start_row = 0;
    } else { // 横向：先行后列 (row = index / COLS, col = index % COLS)
        start_row = 0;
        start_col = 0;
    }

    // 在启动时立即点亮第一个点
    g_static_bitmap[start_col] |= (1 << start_row);
    // 【修改】使用配置的默认亮度
    tm1640_send_data(g_static_bitmap, TM1640_DEFAULT_BRIGHTNESS_CMD);
    
    g_last_update_timer = timer_read();
}

// 【修改】display_bitmap 接受的 brightness 仍是 TM1640 命令格式
void tm1640_display_bitmap(const uint8_t *bitmap_data, tm1640_brightness_t brightness) {
    if (bitmap_data == NULL) return;

    tm1640_stop_current_effect(); 
    
    memcpy(g_static_bitmap, bitmap_data, TM1640_COLS);
    tm1640_send_data(g_static_bitmap, brightness);
    
    g_current_effect = TM1640_EFFECT_STATIC;
}

// -------------------------- QMK 任务处理：非阻塞式 --------------------------
void tm1640_task(void) {
    // 联合判断，避免重复计时器检查
    if (g_current_effect == TM1640_EFFECT_NONE || timer_elapsed(g_last_update_timer) < (g_current_effect == TM1640_EFFECT_BLINK ? TM1640_BLINK_INTERVAL : TM1640_RUNNING_SPEED)) {
        return;
    }
    
    g_last_update_timer = timer_read();
    
    const uint8_t TOTAL_PIXELS = TM1640_ROWS * TM1640_COLS;
    
    switch (g_current_effect) {
        case TM1640_EFFECT_BLINK:
            if (g_blink_step >= TM1640_BLINK_COUNT * 2) {
                tm1640_stop_current_effect();
                break;
            }
            
            g_blink_step++;
            
            // 奇数亮，偶数灭
            if (g_blink_step % 2 != 0) { 
                // 【修改】使用配置的默认亮度
                tm1640_send_data(g_static_bitmap, TM1640_DEFAULT_BRIGHTNESS_CMD);
            } else { 
                tm1640_display_off();
            }
            break;

        case TM1640_EFFECT_RUNNING_LIGHT:
            
            // 1. 熄灭当前点 (g_running_index)
            uint8_t prev_col, prev_row;
            if (TM1640_RUNNING_DIRECTION == 0) { // 纵向
                prev_col = g_running_index / TM1640_ROWS;
                prev_row = g_running_index % TM1640_ROWS;
            } else { // 横向
                prev_row = g_running_index / TM1640_COLS;
                prev_col = g_running_index % TM1640_COLS;
            }
            g_static_bitmap[prev_col] &= ~(1 << prev_row);

            // 2. 推进到下一个点
            g_running_index++;
            
            if (g_running_index >= TOTAL_PIXELS) {
                tm1640_stop_current_effect();
                break;
            }
            
            // 3. 点亮新点 (g_running_index)
            uint8_t next_col, next_row;
            if (TM1640_RUNNING_DIRECTION == 0) { // 纵向
                next_col = g_running_index / TM1640_ROWS;
                next_row = g_running_index % TM1640_ROWS;
            } else { // 横向
                next_row = g_running_index / TM1640_COLS;
                next_col = g_running_index % TM1640_COLS;
            }
            g_static_bitmap[next_col] |= (1 << next_row);
            
            // 4. 发送数据
            // 【修改】使用配置的默认亮度
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