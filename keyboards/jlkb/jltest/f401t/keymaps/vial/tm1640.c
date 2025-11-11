#include "tm1640.h"
#include "timer.h"
#include <stdbool.h>

// QMK GPIO操作宏
#define TM1640_DIN_HIGH()   writePinHigh(TM1640_DIN_PIN)
#define TM1640_DIN_LOW()    writePinLow(TM1640_DIN_PIN)
#define TM1640_SCLK_HIGH()  writePinHigh(TM1640_SCLK_PIN)
#define TM1640_SCLK_LOW()   writePinLow(TM1640_SCLK_PIN)

// -------------------------- 全局变量 --------------------------
static volatile tm1640_effect_type_t g_current_effect = TM1640_EFFECT_NONE;
static volatile bool g_effect_interrupt = false; // 中断标志

// -------------------------- 内部时序和延时 --------------------------
static inline void tm1640_delay(void) {
    for (volatile uint8_t i = 0; i < 10; i++);
}

// 毫秒延时（循环内实时检测中断）
static bool tm1640_delay_ms_with_check(uint16_t ms) {
    for (uint16_t t = 0; t < ms; t++) {
        if (g_effect_interrupt) return false; 
        wait_ms(1);
    }
    return true;
}

// -------------------------- TM1640 基础通信 --------------------------
static void tm1640_start(void) {
    TM1640_SCLK_HIGH(); tm1640_delay();
    TM1640_DIN_HIGH(); tm1640_delay();
    TM1640_DIN_LOW(); tm1640_delay();
    TM1640_SCLK_LOW(); tm1640_delay();
}

static void tm1640_stop(void) {
    TM1640_DIN_LOW(); tm1640_delay();
    TM1640_SCLK_HIGH(); tm1640_delay();
    TM1640_DIN_HIGH(); tm1640_delay();
    TM1640_SCLK_LOW(); tm1640_delay();
}

static void tm1640_send_byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        TM1640_SCLK_LOW(); tm1640_delay();
        
        (data & 0x01) ? TM1640_DIN_HIGH() : TM1640_DIN_LOW();
        tm1640_delay();
        
        TM1640_SCLK_HIGH(); tm1640_delay();
        data >>= 1;
    }
    TM1640_SCLK_LOW(); tm1640_delay();
    TM1640_DIN_LOW();
}

void tm1640_send_data(const uint8_t *data, tm1640_brightness_t brightness) {
    if (data == NULL) return;
    
    tm1640_start();
    tm1640_send_byte(0x40); 
    tm1640_stop();
    
    tm1640_start();
    tm1640_send_byte(0xC0); 
    tm1640_delay();
    
    for (uint8_t i = 0; i < TM1640_COLS; i++) {
        if (g_effect_interrupt) break; 
        tm1640_send_byte(data[i]);
    }
    tm1640_stop();
    
    if (!g_effect_interrupt) {
        tm1640_start();
        tm1640_send_byte(brightness); 
        tm1640_stop();
    }
}

void tm1640_display_off(void) {
    tm1640_start();
    tm1640_send_byte(0x80); 
    tm1640_stop();
}

// -------------------------- 外部接口实现 --------------------------
void tm1640_stop_current_effect(void) {
    g_effect_interrupt = true;
    tm1640_display_off();
    g_current_effect = TM1640_EFFECT_NONE; 
    g_effect_interrupt = false; 
}

static void tm1640_blink_effect(void) {
    g_current_effect = TM1640_EFFECT_BLINK; 
    uint8_t blink_buf[TM1640_COLS] = {0};
    
    for (uint8_t i = 0; i < TM1640_COLS; i++) {
        blink_buf[i] = 0xFF; 
    }
    
    for (uint8_t i = 0; i < TM1640_BLINK_COUNT; i++) {
        if (g_effect_interrupt) break; 
        
        tm1640_send_data(blink_buf, TM1640_BRIGHTNESS_14_16);
        if (!tm1640_delay_ms_with_check(TM1640_BLINK_INTERVAL)) break;
        
        if (g_effect_interrupt) break;
        
        tm1640_display_off();
        if (!tm1640_delay_ms_with_check(TM1640_BLINK_INTERVAL)) break;
    }
    
    if (g_current_effect == TM1640_EFFECT_BLINK) {
        tm1640_display_off();
        g_current_effect = TM1640_EFFECT_NONE; 
    }
}

static void tm1640_running_light_effect(void) {
    g_current_effect = TM1640_EFFECT_RUNNING_LIGHT;
    uint8_t light_buf[TM1640_COLS] = {0};
    const uint8_t SINGLE_PIXEL = 0x01;
    
    for (uint8_t row = 0; row < TM1640_ROWS; row++) {
        for (uint8_t col = 0; col < TM1640_COLS; col++) {
            if (g_effect_interrupt) break;
            
            light_buf[col] = SINGLE_PIXEL << row; 
            tm1640_send_data(light_buf, TM1640_BRIGHTNESS_14_16);
            if (!tm1640_delay_ms_with_check(TM1640_RUNNING_SPEED)) break;
            
            light_buf[col] = 0; 
        }
        if (g_effect_interrupt) break;
    }
    
    if (g_current_effect == TM1640_EFFECT_RUNNING_LIGHT) {
        tm1640_display_off();
        g_current_effect = TM1640_EFFECT_NONE; 
    }
}

void tm1640_start_blink(void) {
    tm1640_stop_current_effect(); 
    tm1640_blink_effect();        
}

void tm1640_start_running_light(void) {
    tm1640_stop_current_effect();     
    tm1640_running_light_effect();    
}

void tm1640_display_bitmap(const uint8_t *bitmap_data, tm1640_brightness_t brightness) {
    if (bitmap_data == NULL) return;

    tm1640_stop_current_effect(); 
    tm1640_send_data(bitmap_data, brightness);
    g_current_effect = TM1640_EFFECT_NONE;
}

void tm1640_init(void) {
    setPinOutput(TM1640_DIN_PIN);
    setPinOutput(TM1640_SCLK_PIN);
    
    TM1640_SCLK_LOW();
    TM1640_DIN_LOW();
    g_current_effect = TM1640_EFFECT_NONE;
    g_effect_interrupt = false;
    tm1640_display_off();
}