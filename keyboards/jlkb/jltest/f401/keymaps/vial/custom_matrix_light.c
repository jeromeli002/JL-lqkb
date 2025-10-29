#include "quantum.h"
#include "custom_matrix_light.h"

// 检查是否定义了 QMK 宏
#ifndef M_PI 
#define M_PI 3.14159265358979323846f
#endif

// 引脚数组定义
static const pin_t row_pins[MATRIX_LIGHT_ROWS] = MATRIX_LIGHT_ROW_PINS;
static const pin_t col_pins[MATRIX_LIGHT_COLS] = MATRIX_LIGHT_COL_PINS;

// 像素状态数组大小由 MATRIX_LIGHT_ROWS 决定
static uint8_t pixel_state_pattern[MATRIX_LIGHT_ROWS] = {0}; 

// 效果控制变量
static matrix_light_effect_t current_effect = MATRIX_EFFECT_NONE;
static uint32_t effect_start_time = 0;
static uint8_t  current_brightness = 100; // 用于亮度和闪烁

#define SCAN_TIME_MS 2000 // 扫描效果持续时间
// 闪烁半周期调整为 500 ms (0.5秒)
#define FLASH_PERIOD_MS 500 // 闪烁半周期（亮/灭持续时间）
#define FLASH_COUNT 3 // 闪烁次数

// 【幽灵灯修正】清空间隙（微秒）
#define DISCHARGE_DELAY_US 5 
#define PWM_MAX_LEVEL 50

// --- 低级GPIO控制 (保留不变) ---
static void set_row_pins_high_z(void) {
    for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++) {
        gpio_set_pin_input(row_pins[i]);
    }
}

static void set_col_pins_high_z(void) {
    for (uint8_t i = 0; i < MATRIX_LIGHT_COLS; i++) {
        gpio_set_pin_input(col_pins[i]);
    }
}

static void drive_row(uint8_t row_idx) {
    gpio_set_pin_output(row_pins[row_idx]);
    gpio_write_pin_low(row_pins[row_idx]);
}

static void drive_col(uint8_t col_idx) {
    gpio_set_pin_output(col_pins[col_idx]);
    gpio_write_pin_high(col_pins[col_idx]);
}

// --- 位翻转函数 (保留不变) ---
static uint8_t reverse_bits_in_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

// --- 矩阵显示逻辑 (50 级 PWM + 幽灵灯修正) ---
static void refresh_matrix_display(uint8_t brightness) {
    if (brightness == 0) {
        set_row_pins_high_z();
        set_col_pins_high_z();
        return;
    }
    
    // 将 0-100 的亮度映射到 0-50 的 PWM 等级
    uint8_t mapped_brightness = (brightness * PWM_MAX_LEVEL) / 100;
    
    for (uint8_t pwm_cycle = 0; pwm_cycle < PWM_MAX_LEVEL; pwm_cycle++) {
        bool should_light = (pwm_cycle < mapped_brightness);

        for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++) {
            uint8_t row_idx = i;
            
            // --------------------- 【幽灵灯修正】清空间隙 ---------------------
            set_row_pins_high_z();
            set_col_pins_high_z();
            wait_us(DISCHARGE_DELAY_US); 
            // -----------------------------------------------------------------

            drive_row(row_idx);

            uint8_t row_data = pixel_state_pattern[i];
            row_data = reverse_bits_in_byte(row_data);

            if (should_light) {
                for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
                    if ((row_data >> col) & 0x01) {
                        drive_col(col);
                    }
                }
            }
            // PWM 维持时间
            wait_us(10); 
        }
    }
}

// --- 效果实现 ---

// 效果1：逐颗点亮 (Scan Init) - 修正最后一颗灯问题
static void handle_scan_init(void) {
    uint32_t elapsed = timer_elapsed(effect_start_time);
    uint16_t total_pixels = (uint16_t)MATRIX_LIGHT_ROWS * MATRIX_LIGHT_COLS;
    
    if (elapsed >= SCAN_TIME_MS) {
        // 效果时间结束：强制点亮所有灯，并退出动画模式。
        memset(pixel_state_pattern, 0xFF, sizeof(pixel_state_pattern)); 
        current_effect = MATRIX_EFFECT_NONE;
        current_brightness = 100;
        return;
    }
    
    // 计算当前应该点亮多少颗灯 (整数除法)
    uint16_t current_pixel_idx = (uint16_t)(((uint32_t)elapsed * total_pixels) / SCAN_TIME_MS);
    
    // 如果 elapsed 接近 SCAN_TIME_MS，强制 current_pixel_idx = total_pixels
    if (elapsed > SCAN_TIME_MS - 2) { 
        current_pixel_idx = total_pixels;
    }

    // 清空整个矩阵
    memset(pixel_state_pattern, 0x00, sizeof(pixel_state_pattern));
    
    // 逐颗点亮
    for (uint16_t k = 0; k < current_pixel_idx; k++) { 
        uint8_t r = k / MATRIX_LIGHT_COLS;
        uint8_t c = k % MATRIX_LIGHT_COLS;
        
        if (r < MATRIX_LIGHT_ROWS && c < MATRIX_LIGHT_COLS) {
            uint8_t flipped_col = (MATRIX_LIGHT_COLS - 1) - c;
            pixel_state_pattern[r] |= (1 << flipped_col);
        }
    }
    
    current_brightness = 100; // 全亮
}

// 效果2：全体闪烁 (Flash Init)
static void handle_flash_init(void) {
    uint32_t elapsed = timer_elapsed(effect_start_time);
    
    // 闪烁总时长 = 3次 * 2 (亮灭) * 500ms = 3000 ms (3秒)
    uint32_t total_flash_time = FLASH_COUNT * 2 * FLASH_PERIOD_MS;
    
    if (elapsed >= total_flash_time) {
        // 效果结束：所有灯熄灭
        current_effect = MATRIX_EFFECT_NONE;
        current_brightness = 100; 
        memset(pixel_state_pattern, 0x00, sizeof(pixel_state_pattern)); 
        return;
    }

    // 计算当前处于哪个半周期 (亮或灭)
    uint32_t half_period_idx = elapsed / FLASH_PERIOD_MS;
    
    // 偶数半周期 (0, 2, 4) -> 亮 (100%)
    bool is_on = (half_period_idx % 2 == 0);
    
    if (is_on) {
        current_brightness = 100;
        // 闪烁时将所有有效行设置为全亮状态
        memset(pixel_state_pattern, 0xFF, sizeof(pixel_state_pattern));
    } else {
        current_brightness = 0;
        // 闪烁时将所有灯设置为全灭状态 
        memset(pixel_state_pattern, 0x00, sizeof(pixel_state_pattern)); 
    }
}


// --- 外部接口 (保留不变) ---

void custom_matrix_light_init(void) {
    set_row_pins_high_z();
    set_col_pins_high_z();
}

void custom_matrix_light_start_effect(matrix_light_effect_t effect) {
    if (effect == MATRIX_EFFECT_NONE) {
        current_effect = MATRIX_EFFECT_NONE;
        current_brightness = 100; 
        return;
    }
    
    if (effect == MATRIX_EFFECT_SCAN_INIT || effect == MATRIX_EFFECT_FLASH_INIT) {
        custom_matrix_light_clear_all(); 
    }

    current_effect = effect;
    effect_start_time = timer_read();
    current_brightness = 100; 
}

void custom_matrix_light_set_pixels(const matrix_pixel_t* points, size_t num_points, bool state) {
    if (current_effect != MATRIX_EFFECT_NONE) return; 

    for (size_t i = 0; i < num_points; i++) {
        uint8_t row = points[i].row;
        uint8_t col = points[i].col;

        if (row < MATRIX_LIGHT_ROWS && col < MATRIX_LIGHT_COLS) {
            uint8_t flipped_col = (MATRIX_LIGHT_COLS - 1) - col;

            if (state) {
                pixel_state_pattern[row] |= (1 << flipped_col);
            } else {
                pixel_state_pattern[row] &= ~(1 << flipped_col);
            }
        }
    }
}

void custom_matrix_light_clear_all(void) {
    memset(pixel_state_pattern, 0x00, sizeof(pixel_state_pattern)); 
    current_effect = MATRIX_EFFECT_NONE; 
    current_brightness = 100;
}

void custom_matrix_light_task(void) {
    uint8_t display_brightness = 100;

    switch (current_effect) {
        case MATRIX_EFFECT_SCAN_INIT:
            handle_scan_init();
            display_brightness = current_brightness;
            break;
        case MATRIX_EFFECT_FLASH_INIT: 
            handle_flash_init();
            display_brightness = current_brightness;
            break;
        case MATRIX_EFFECT_NONE:
        default:
            break;
    }
    
    refresh_matrix_display(display_brightness);
}