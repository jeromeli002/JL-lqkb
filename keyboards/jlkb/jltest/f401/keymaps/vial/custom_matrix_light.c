#include "quantum.h"
#include "custom_matrix_light.h"

// --- 变量和宏定义 ---
static const pin_t row_pins[MATRIX_LIGHT_ROWS] = MATRIX_LIGHT_ROW_PINS;
static const pin_t col_pins[MATRIX_LIGHT_COLS] = MATRIX_LIGHT_COL_PINS;
static uint8_t pixel_state[MATRIX_LIGHT_ROWS] = {0}; 
static matrix_light_effect_t current_effect = MATRIX_EFFECT_NONE;
static uint32_t effect_start_time = 0;

#define SCAN_TIME_MS 2000 
#define FLASH_PERIOD_MS 500 
#define FLASH_COUNT 3 
#define PWM_MAX_LEVEL 20
#define HOLD_US 30
#define SPECIAL_HOLD_US 120
#define DISCHARGE_US 20

#define COLS_MASK ((1 << MATRIX_LIGHT_COLS) - 1) 

// --- 低级GPIO控制 ---
static void set_all_high_z(void) {
    for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++)
        gpio_set_pin_input(row_pins[i]);
    for (uint8_t i = 0; i < MATRIX_LIGHT_COLS; i++)
        gpio_set_pin_input(col_pins[i]);
}

static void drive_row(uint8_t row) {
    gpio_set_pin_output(row_pins[row]);
    gpio_write_pin_low(row_pins[row]);
}

static void drive_col(uint8_t col) {
    gpio_set_pin_output(col_pins[col]);
    gpio_write_pin_high(col_pins[col]);
}

// --- 矩阵显示逻辑 ---
static void refresh_display(uint8_t brightness) {
    if (brightness == 0) {
        set_all_high_z();
        return;
    }
    
    uint8_t pwm = (brightness * PWM_MAX_LEVEL) / 100;
    
    for (uint8_t cycle = 0; cycle < PWM_MAX_LEVEL; cycle++) {
        if (cycle >= pwm) continue;
        
        // 单独处理最后一行第一列
        if (pixel_state[MATRIX_LIGHT_ROWS - 1] & 0x01 && cycle < pwm + 2) {
            set_all_high_z();
            wait_us(DISCHARGE_US);
            drive_row(MATRIX_LIGHT_ROWS - 1);
            wait_us(DISCHARGE_US);
            drive_col(0);
            wait_us(SPECIAL_HOLD_US);
            set_all_high_z();
            wait_us(DISCHARGE_US);
        }
        
        // 处理其他像素
        for (uint8_t row = 0; row < MATRIX_LIGHT_ROWS; row++) {
            if (pixel_state[row] == 0) continue;
            
            set_all_high_z();
            wait_us(DISCHARGE_US);
            drive_row(row);
            
            for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
                if (row == MATRIX_LIGHT_ROWS - 1 && col == 0) continue;
                if ((pixel_state[row] >> col) & 0x01)
                    drive_col(col);
            }
            
            wait_us(HOLD_US);
        }
    }
    set_all_high_z();
}

// --- 效果实现 ---
static void handle_scan_effect(void) {
    uint32_t elapsed = timer_elapsed(effect_start_time);
    uint16_t total = (uint16_t)MATRIX_LIGHT_ROWS * MATRIX_LIGHT_COLS;
    
    if (elapsed >= SCAN_TIME_MS) {
        memset(pixel_state, 0, sizeof(pixel_state)); 
        current_effect = MATRIX_EFFECT_NONE;
        return;
    }
    
    uint16_t idx = (elapsed >= SCAN_TIME_MS - 1) ? total : (uint16_t)((elapsed * total) / SCAN_TIME_MS);

    memset(pixel_state, 0, sizeof(pixel_state));
    for (uint16_t k = 0; k < idx; k++) { 
        uint8_t r = k / MATRIX_LIGHT_COLS;
        uint8_t c = k % MATRIX_LIGHT_COLS;
        if (r < MATRIX_LIGHT_ROWS && c < MATRIX_LIGHT_COLS)
            pixel_state[r] |= (1 << c);
    }
}

static void handle_flash_effect(void) {
    uint32_t elapsed = timer_elapsed(effect_start_time);
    uint32_t total = FLASH_COUNT * 2 * FLASH_PERIOD_MS;
    
    if (elapsed >= total) {
        current_effect = MATRIX_EFFECT_NONE;
        memset(pixel_state, 0, sizeof(pixel_state)); 
        return;
    }

    bool on = ((elapsed / FLASH_PERIOD_MS) % 2 == 0);
    if (on) {
        for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++)
            pixel_state[i] = COLS_MASK;
    } else {
        memset(pixel_state, 0, sizeof(pixel_state)); 
    }
}

// --- 外部接口 ---
void custom_matrix_light_init(void) {
    set_all_high_z();
}

void custom_matrix_light_start_effect(matrix_light_effect_t effect) {
    if (effect == MATRIX_EFFECT_NONE) {
        current_effect = MATRIX_EFFECT_NONE;
        return;
    }
    
    if (effect == MATRIX_EFFECT_SCAN_INIT || effect == MATRIX_EFFECT_FLASH_INIT)
        memset(pixel_state, 0, sizeof(pixel_state)); 

    current_effect = effect;
    effect_start_time = timer_read();
}

void custom_matrix_light_set_pixels(const matrix_pixel_t* points, size_t num, bool state) {
    if (current_effect != MATRIX_EFFECT_NONE) return; 

    for (size_t i = 0; i < num; i++) {
        uint8_t row = points[i].row;
        uint8_t col = points[i].col;
        if (row < MATRIX_LIGHT_ROWS && col < MATRIX_LIGHT_COLS) {
            if (state)
                pixel_state[row] |= (1 << col);
            else
                pixel_state[row] &= ~(1 << col);
        }
    }
}

void custom_matrix_light_clear_all(void) {
    memset(pixel_state, 0, sizeof(pixel_state)); 
    current_effect = MATRIX_EFFECT_NONE; 
    set_all_high_z();
}

void custom_matrix_light_task(void) {
    switch (current_effect) {
        case MATRIX_EFFECT_SCAN_INIT:
            handle_scan_effect();
            break;
        case MATRIX_EFFECT_FLASH_INIT: 
            handle_flash_effect();
            break;
        default:
            break;
    }
    
    refresh_display(100);
}