#include "quantum.h"
#include "custom_matrix_light.h"

// --- 静态变量和宏定义 ---
static const pin_t row_pins[MATRIX_LIGHT_ROWS] = MATRIX_LIGHT_ROW_PINS;
static const pin_t col_pins[MATRIX_LIGHT_COLS] = MATRIX_LIGHT_COL_PINS;
static uint8_t pixel_state[MATRIX_LIGHT_ROWS] = {0}; 
static matrix_light_effect_t current_effect = MATRIX_EFFECT_NONE;
static uint32_t effect_start_time = 0;

#define SCAN_TIME_MS 2000 
#define FLASH_PERIOD_MS 500 
#define FLASH_COUNT  MATRIX_LIGHT_FLASH_COUNT  // 引用配置的闪烁次数
#define PWM_MAX_LEVEL 20
#define HOLD_US 30
#define SPECIAL_HOLD_US 200  // 延长特殊像素保持时间（修复点亮问题）
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
#if MATRIX_LIGHT_LEVEL_CONFIG == C2R
    gpio_write_pin_low(row_pins[row]);  // C2R：行低电平
#elif MATRIX_LIGHT_LEVEL_CONFIG == R2C
    gpio_write_pin_high(row_pins[row]); // R2C：行高电平
#endif
}

static void drive_col(uint8_t col) {
    gpio_set_pin_output(col_pins[col]);
#if MATRIX_LIGHT_LEVEL_CONFIG == C2R
    gpio_write_pin_high(col_pins[col]); // C2R：列高电平
#elif MATRIX_LIGHT_LEVEL_CONFIG == R2C
    gpio_write_pin_low(col_pins[col]);  // R2C：列低电平
#endif
}

// --- 矩阵显示逻辑（修复特殊像素点亮问题）---
static void refresh_display(void) {
    uint8_t brightness = MATRIX_LIGHT_DEFAULT_BRIGHTNESS;
    
    if (brightness == 0) {
        set_all_high_z();
        return;
    }
    
    uint8_t pwm = (brightness * PWM_MAX_LEVEL) / 100;
    
    for (uint8_t cycle = 0; cycle < PWM_MAX_LEVEL; cycle++) {
        if (cycle >= pwm) continue;
        
        // 修复：特殊像素（最后一行第一列）单独驱动
#if MATRIX_LIGHT_ENABLE_SPECIAL_PIXEL == 1
        uint8_t special_row = MATRIX_LIGHT_ROWS - 1;
        uint8_t special_col = 0;
        // 只要像素状态为点亮，就驱动（移除cycle < pwm+2的限制）
        if (pixel_state[special_row] & (1 << special_col)) {
            set_all_high_z();
            wait_us(DISCHARGE_US);
            drive_row(special_row);      // 先驱动行，稳定电平
            wait_us(50);                 // 延长行电平稳定时间
            drive_col(special_col);      // 再驱动列
            wait_us(SPECIAL_HOLD_US);    // 延长保持时间，确保导通
            set_all_high_z();
            wait_us(DISCHARGE_US);
        }
#endif
        
        // 处理其他像素（跳过特殊像素，避免重复驱动）
        for (uint8_t row = 0; row < MATRIX_LIGHT_ROWS; row++) {
            if (pixel_state[row] == 0) continue;
            
            set_all_high_z();
            wait_us(DISCHARGE_US);
            drive_row(row);
            
            for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
#if MATRIX_LIGHT_ENABLE_SPECIAL_PIXEL == 1
                // 跳过特殊像素（已单独处理）
                if (row == MATRIX_LIGHT_ROWS - 1 && col == 0) continue;
#endif
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
    uint16_t total_pixels = (uint16_t)MATRIX_LIGHT_ROWS * MATRIX_LIGHT_COLS;
    
    if (elapsed >= SCAN_TIME_MS) {
        memset(pixel_state, 0, sizeof(pixel_state));
        current_effect = MATRIX_EFFECT_NONE;
        return;
    }
    
    // 计算当前应点亮的像素数（修正中文变量名）
    uint16_t active_pixels = (elapsed >= SCAN_TIME_MS - 1) ? total_pixels : (uint16_t)((elapsed * total_pixels) / SCAN_TIME_MS);
    
    memset(pixel_state, 0, sizeof(pixel_state));
    for (uint16_t k = 0; k < active_pixels; k++) {
        uint8_t r = k / MATRIX_LIGHT_COLS;
        uint8_t c = k % MATRIX_LIGHT_COLS;
        if (r < MATRIX_LIGHT_ROWS && c < MATRIX_LIGHT_COLS)
            pixel_state[r] |= (1 << c);
    }
}

static void handle_flash_effect(void) {
    uint32_t elapsed = timer_elapsed(effect_start_time);
    uint32_t total_duration = FLASH_COUNT * 2 * FLASH_PERIOD_MS;
    
    if (elapsed >= total_duration) {
        current_effect = MATRIX_EFFECT_NONE;
        memset(pixel_state, 0, sizeof(pixel_state));
        return;
    }
    
    // 交替点亮/熄灭
    bool is_on = ((elapsed / FLASH_PERIOD_MS) % 2 == 0);
    if (is_on) {
        for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++)
            pixel_state[i] = COLS_MASK;
    } else {
        memset(pixel_state, 0, sizeof(pixel_state));
    }
}

// --- 外部接口实现 ---
void custom_matrix_light_init(void) {
    set_all_high_z();
}

void custom_matrix_light_start_effect(matrix_light_effect_t effect) {
    if (effect == MATRIX_EFFECT_NONE) {
        current_effect = MATRIX_EFFECT_NONE;
        return;
    }
    
    // 效果启动前清空像素状态
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
    // 处理当前效果
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
    
    // 刷新显示
    refresh_display();
}