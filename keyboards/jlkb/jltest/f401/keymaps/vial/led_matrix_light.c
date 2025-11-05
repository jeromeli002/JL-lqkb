#include "quantum.h"
#include "led_matrix_light.h"

#ifndef M_PI 
#define M_PI 3.14159265358979323846f
#endif

// --- 变量和宏定义 ---
static const pin_t row_pins[MATRIX_LIGHT_ROWS] = MATRIX_LIGHT_ROW_PINS;
static const pin_t col_pins[MATRIX_LIGHT_COLS] = MATRIX_LIGHT_COL_PINS;
static uint8_t pixel_state_pattern[MATRIX_LIGHT_ROWS] = {0}; 
static matrix_light_effect_t current_effect = MATRIX_EFFECT_NONE;
static uint32_t effect_start_time = 0;
static uint8_t  current_brightness = 100;

#define SCAN_TIME_MS 2000 
#define FLASH_PERIOD_MS 500 
#define FLASH_COUNT 3 

#define DISCHARGE_DELAY_US 5 // 幽灵灯修正 (如果仍有微亮，可以增大到 10 或 20)
#define PWM_MAX_LEVEL 50

#define COLS_MASK ((1 << MATRIX_LIGHT_COLS) - 1) 

// --- 低级GPIO控制 (保持不变) ---
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

// 移除不必要的 8 位位反转函数，因为我们只需要在设置像素时进行列逻辑映射。
/*
static uint8_t reverse_bits_in_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}
*/

// --- 矩阵显示逻辑 (已修复兼容性问题) ---
static void refresh_matrix_display(uint8_t brightness) {
    if (brightness == 0) {
        set_row_pins_high_z();
        set_col_pins_high_z();
        return;
    }
    
    uint8_t mapped_brightness = (brightness * PWM_MAX_LEVEL) / 100;
    
    for (uint8_t pwm_cycle = 0; pwm_cycle < PWM_MAX_LEVEL; pwm_cycle++) {
        bool should_light = (pwm_cycle < mapped_brightness);

        for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++) {
            uint8_t row_idx = i;
            
            set_row_pins_high_z();
            set_col_pins_high_z();
            wait_us(DISCHARGE_DELAY_US); 

            drive_row(row_idx);

            uint8_t row_data = pixel_state_pattern[i];
            // 【★ 关键修复：移除对整个 8 位数据的反转】
            // row_data = reverse_bits_in_byte(row_data); 

            if (should_light) {
                // 仅扫描 MATRIX_LIGHT_COLS 指定的有效列
                for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
                    if ((row_data >> col) & 0x01) {
                        drive_col(col);
                    }
                }
            }
            wait_us(10); 
        }
    }
}

// --- 效果实现 ---

// 效果1：逐颗点亮 (Scan Init) - (保持与原逻辑一致，仅依赖于 MATRIX_LIGHT_COLS 的列映射)
static void handle_scan_init(void) {
    uint32_t elapsed = timer_elapsed(effect_start_time);
    uint16_t total_pixels = (uint16_t)MATRIX_LIGHT_ROWS * MATRIX_LIGHT_COLS;
    
    // 【修正：动画结束，全部灭掉】
    if (elapsed >= SCAN_TIME_MS) {
        memset(pixel_state_pattern, 0x00, sizeof(pixel_state_pattern)); 
        current_effect = MATRIX_EFFECT_NONE;
        current_brightness = 100;
        return;
    }
    
    // 计算当前应该点亮多少颗灯 (逐颗逻辑)
    uint16_t current_pixel_idx = (uint16_t)(((uint32_t)elapsed * total_pixels) / SCAN_TIME_MS);
    
    // 确保在动画结束前 1 毫秒内，强制点亮所有灯，防止截断遗漏。
    if (current_pixel_idx < total_pixels && elapsed >= SCAN_TIME_MS - 1) { 
        current_pixel_idx = total_pixels;
    }

    memset(pixel_state_pattern, 0x00, sizeof(pixel_state_pattern));
    
    // 逐颗点亮
    for (uint16_t k = 0; k < current_pixel_idx; k++) { 
        uint8_t r = k / MATRIX_LIGHT_COLS;
        uint8_t c = k % MATRIX_LIGHT_COLS;
        
        if (r < MATRIX_LIGHT_ROWS && c < MATRIX_LIGHT_COLS) {
            // 【列映射逻辑保持不变，适用于非 8x8 矩阵】
            uint8_t flipped_col = (MATRIX_LIGHT_COLS - 1) - c; 
            pixel_state_pattern[r] |= (1 << flipped_col);
        }
    }
    
    current_brightness = 100; 
}

// 效果2：全体闪烁 (Flash Init) - (依赖 COLS_MASK，兼容非 8x8 矩阵)
static void handle_flash_init(void) {
    uint32_t elapsed = timer_elapsed(effect_start_time);
    uint32_t total_flash_time = FLASH_COUNT * 2 * FLASH_PERIOD_MS;
    
    if (elapsed >= total_flash_time) {
        current_effect = MATRIX_EFFECT_NONE;
        current_brightness = 100; 
        memset(pixel_state_pattern, 0x00, sizeof(pixel_state_pattern)); 
        return;
    }

    uint32_t half_period_idx = elapsed / FLASH_PERIOD_MS;
    bool is_on = (half_period_idx % 2 == 0);
    
    if (is_on) {
        current_brightness = 100;
        uint8_t mask = COLS_MASK; // 仅设置 MATRIX_LIGHT_COLS 范围内的有效位
        for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++) {
            pixel_state_pattern[i] = mask;
        }
    } else {
        current_brightness = 0;
        memset(pixel_state_pattern, 0x00, sizeof(pixel_state_pattern)); 
    }
}


// --- 外部接口 (保持不变) ---

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
            // 【列映射逻辑保持不变，适用于非 8x8 矩阵】
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