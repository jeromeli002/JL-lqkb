#include "quantum.h"
#include "custom_matrix_light.h"
#include "custom_matrix_chars.h" // 字符点阵数据仍然是 8x8

// 引脚数组定义
static const pin_t row_pins[MATRIX_LIGHT_ROWS] = MATRIX_LIGHT_ROW_PINS;
static const pin_t col_pins[MATRIX_LIGHT_COLS] = MATRIX_LIGHT_COL_PINS;

// 新增：两个独立的模式数组，保持 8x8 大小以兼容现有字符/图案接口
// 如果 MATRIX_LIGHT_ROWS 或 MATRIX_LIGHT_COLS > 8，字符和自定义图案只会显示在左上角 8x8 区域
static uint8_t char_display_pattern[8] = {0};
static uint8_t custom_pixel_current_pattern[8] = {0};

// 动画相关变量
static uint32_t last_action_timer = 0;

#if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
static bool       is_raining       = false;
static uint32_t   last_rain_update = 0;
// rain_pattern 动态大小，以适应整个矩阵
static uint8_t      rain_pattern[MATRIX_LIGHT_ROWS][MATRIX_LIGHT_COLS] = {0};
#endif

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

// --- 位翻转函数 (保持不变) ---
static uint8_t reverse_bits_in_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

// --- 像素点旋转函数 (保持不变) ---
static void apply_rotation(uint8_t original_row, uint8_t original_col, uint8_t* new_row, uint8_t* new_col) {
    switch (MATRIX_LIGHT_ROTATION) {
        case MATRIX_LIGHT_ROTATION_90: // 顺时针90度
            *new_row = original_col;
            *new_col = (MATRIX_LIGHT_ROWS - 1) - original_row;
            break;
        case MATRIX_LIGHT_ROTATION_180: // 顺时针180度
            *new_row = (MATRIX_LIGHT_ROWS - 1) - original_row;
            *new_col = (MATRIX_LIGHT_COLS - 1) - original_col;
            break;
        case MATRIX_LIGHT_ROTATION_270: // 顺时针270度 (或逆时针90度)
            *new_row = (MATRIX_LIGHT_COLS - 1) - original_col;
            *new_col = original_row;
            break;
        case MATRIX_LIGHT_ROTATION_NONE: // 不旋转
        default:
            *new_row = original_row;
            *new_col = original_col;
            break;
    }
}

// --- 矩阵显示逻辑 (保持不变) ---
static void refresh_matrix_display(void) {
    // 临时的合并图案数组，大小应为实际行数，每行是一个字节
    uint8_t combined_pattern[MATRIX_LIGHT_ROWS] = {0};

    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    if (is_raining) {
        // 如果正在下雨，则只显示雨滴图案
        for (uint8_t r = 0; r < MATRIX_LIGHT_ROWS; r++) {
            for (uint8_t c = 0; c < MATRIX_LIGHT_COLS; c++) {
                if (rain_pattern[r][c]) {
                    combined_pattern[r] |= (1 << c);
                }
            }
        }
    } else
    #endif
    {
        // 否则，合并字符显示和自定义像素显示
        // 注意：char_display_pattern 和 custom_pixel_current_pattern 仍然是 8x8
        // 所以我们只合并到前8行
        for (uint8_t r = 0; r < 8 && r < MATRIX_LIGHT_ROWS; r++) { // 仅处理前 8 行或矩阵实际行数
            combined_pattern[r] = char_display_pattern[r] | custom_pixel_current_pattern[r];
        }
    }

    // 创建一个应用了旋转后的最终图案，大小应为实际行数
    uint8_t final_display_pattern[MATRIX_LIGHT_ROWS] = {0};

    // 应用旋转
    for (uint8_t r = 0; r < MATRIX_LIGHT_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_LIGHT_COLS; c++) {
            if ((combined_pattern[r] >> c) & 0x01) { // 如果原始位置的像素是亮的
                uint8_t new_row, new_col;
                apply_rotation(r, c, &new_row, &new_col); // 计算旋转后的新位置
                // 确保新位置在有效范围内
                if (new_row < MATRIX_LIGHT_ROWS && new_col < MATRIX_LIGHT_COLS) {
                    final_display_pattern[new_row] |= (1 << new_col); // 设置旋转后位置的像素
                }
            }
        }
    }

    for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++) {
        uint8_t actual_row_idx;
        #if MATRIX_LIGHT_FLIP_VERTICAL == 1
            actual_row_idx = (MATRIX_LIGHT_ROWS - 1) - i;
        #else
            actual_row_idx = i;
        #endif

        set_row_pins_high_z();
        set_col_pins_high_z();

        drive_row(i);

        uint8_t row_data = final_display_pattern[actual_row_idx];

        #if MATRIX_LIGHT_FLIP_HORIZONTAL == 0
            row_data = reverse_bits_in_byte(row_data); // 仍然是8位反转
        #endif

        for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
            if ((row_data >> col) & 0x01) {
                drive_col(col);
            }
        }
        wait_us(100);
    }
}

// --- 外部接口 ---

void custom_matrix_light_init(void) {
    set_row_pins_high_z();
    set_col_pins_high_z();
    last_action_timer = timer_read();
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    srand(timer_read());
    #endif
}

void custom_matrix_light_set_char(char c) {
    // 在设置字符时，清空自定义像素层 (依然是 8x8)
    memset(custom_pixel_current_pattern, 0x00, sizeof(custom_pixel_current_pattern));

    if (c >= ' ' && c <= '~') {
        // char_display_pattern 仍然是 8x8
        memcpy(char_display_pattern, character_patterns[c - ' '], sizeof(char_display_pattern));
    } else {
        memcpy(char_display_pattern, SMILEY_FACE_FOR_UNKNOWN, sizeof(char_display_pattern));
    }
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    is_raining = false; // 有新字符显示，停止下雨
    #endif
    last_action_timer = timer_read(); // 重置计时器
}

// --- MODIFIED FUNCTION ---
void custom_matrix_light_set_pattern(const uint8_t pattern[8]) {
    // 在设置自定义图案时，清空字符显示层
    memset(char_display_pattern, 0x00, sizeof(char_display_pattern));

    // 对传入的每一行图案数据执行位反转，以匹配显示逻辑
    for (uint8_t i = 0; i < 8 && i < MATRIX_LIGHT_ROWS; i++) {
        // 假设传入的 pattern[i] 是逻辑图案, 我们需要存储它的镜像版本
        custom_pixel_current_pattern[i] = reverse_bits_in_byte(pattern[i]);
    }
    
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    is_raining = false; // 有新图案显示，停止下雨
    #endif
    last_action_timer = timer_read();
}

#if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
void custom_matrix_light_start_rain(void) {
    is_raining = true;
    last_rain_update = timer_read();
    // 启动下雨时，清空字符层和自定义像素层 (依然是 8x8)
    memset(char_display_pattern, 0x00, sizeof(char_display_pattern));
    memset(custom_pixel_current_pattern, 0x00, sizeof(custom_pixel_current_pattern));
    // 清空雨滴图案，适配整个矩阵大小
    for (uint8_t r = 0; r < MATRIX_LIGHT_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_LIGHT_COLS; c++) {
            rain_pattern[r][c] = (rand() % 10 == 0) ? 1 : 0; // 10%的概率出现雨滴
        }
    }
    last_action_timer = timer_read();
}
#endif

// --- MODIFIED FUNCTION ---
void custom_matrix_light_set_pixels(const matrix_pixel_t* points, size_t num_points, bool state) {
    bool pixel_changed = false;

    // 在设置单个像素时，清空字符显示层
    memset(char_display_pattern, 0x00, sizeof(char_display_pattern));

    for (size_t i = 0; i < num_points; i++) {
        uint8_t row = points[i].row;
        uint8_t col = points[i].col;

        // 检查坐标是否在 8x8 范围内
        if (row < 8 && col < 8) {
            uint8_t old_pattern_row = custom_pixel_current_pattern[row];

            // 为了抵消 refresh_matrix_display 中的 reverse_bits_in_byte,
            // 我们在这里提前对列进行翻转。
            // 假设 MATRIX_LIGHT_COLS 是 8。
            uint8_t flipped_col = (MATRIX_LIGHT_COLS - 1) - col;

            if (state) {
                custom_pixel_current_pattern[row] |= (1 << flipped_col);
            } else {
                custom_pixel_current_pattern[row] &= ~(1 << flipped_col);
            }

            if (old_pattern_row != custom_pixel_current_pattern[row]) {
                pixel_changed = true;
            }
        }
    }

    if (pixel_changed) {
        #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
        is_raining = false; // 如果有像素被改变，停止下雨效果
        #endif
        last_action_timer = timer_read(); // 重置计时器
    }
}

#if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
void custom_matrix_light_stop_rain(void) {
    is_raining = false;
    // 清空雨滴图案，适配整个矩阵大小
    for (uint8_t r = 0; r < MATRIX_LIGHT_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_LIGHT_COLS; c++) {
            rain_pattern[r][c] = 0;
        }
    }
}

// 模拟下雨效果：雨滴向下移动，新雨滴从顶部生成
static void update_rain_effect(void) {
    // 每次更新时，将所有雨滴向下移动一行
    for (int8_t r = MATRIX_LIGHT_ROWS - 1; r >= 0; r--) { // 循环适应行数
        for (uint8_t c = 0; c < MATRIX_LIGHT_COLS; c++) { // 循环适应列数
            if (r == 0) {
                // 最上面一行随机生成新的雨滴
                rain_pattern[r][c] = (rand() % 10 == 0) ? 1 : 0;
            } else {
                rain_pattern[r][c] = rain_pattern[r - 1][c];
            }
        }
    }
}
#endif

void custom_matrix_light_clear_all(void) {
    memset(char_display_pattern, 0x00, sizeof(char_display_pattern));         // 清空字符显示层 (8x8)
    memset(custom_pixel_current_pattern, 0x00, sizeof(custom_pixel_current_pattern)); // 清空自定义像素显示层 (8x8)
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    is_raining = false;                                       // 停止下雨效果
    // 清空雨滴图案，适配整个矩阵大小
    for (uint8_t r = 0; r < MATRIX_LIGHT_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_LIGHT_COLS; c++) {
            rain_pattern[r][c] = 0; // 清空雨滴图案
        }
    }
    #endif
    last_action_timer = timer_read(); // 重置操作计时器
}

void custom_matrix_light_task(void) {
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    if (!is_raining && timer_elapsed(last_action_timer) > MATRIX_LIGHT_RAIN_TIMEOUT_MS) {
        custom_matrix_light_start_rain();
    }

    if (is_raining) {
        // 每隔一定时间更新下雨效果 (例如 100ms)
        if (timer_elapsed(last_rain_update) > 100) {
            update_rain_effect();
            last_rain_update = timer_read();
        }
    }
    #endif

    refresh_matrix_display();
}