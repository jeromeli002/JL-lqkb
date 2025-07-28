#include "quantum.h" // 包含 QMK 核心头文件，提供 GPIO 函数声明
#include "custom_matrix_light.h"
#include "custom_matrix_chars.h" // 包含字符点阵数据

// 引脚数组定义
static const pin_t row_pins[MATRIX_LIGHT_ROWS] = MATRIX_LIGHT_ROW_PINS;
static const pin_t col_pins[MATRIX_LIGHT_COLS] = MATRIX_LIGHT_COL_PINS;

// 当前要显示的8x8点阵数据
static uint8_t current_pattern[8] = {0};

// 动画相关变量
// last_action_timer 用于记录最后一次操作时间，不受下雨效果开关控制
static uint32_t last_action_timer = 0;

#if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
static bool       is_raining        = false;
static uint32_t   last_rain_update  = 0;
static uint8_t    rain_pattern[8][8] = {0}; // 用于下雨效果的点阵
#endif

// -----------------------------------------------------------------------------
// 低级GPIO控制
// row接负极 (下拉，然后高阻)，col接正极 (上拉，然后高阻)
// 为了实现高阻抗，需要先将引脚设置为输出，然后驱动到相应电平，再切换为输入（高阻抗）
// -----------------------------------------------------------------------------

// 设置行引脚为高阻态
static void set_row_pins_high_z(void) {
    for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++) {
        gpio_set_pin_input(row_pins[i]);
    }
}

// 设置列引脚为高阻态
static void set_col_pins_high_z(void) {
    for (uint8_t i = 0; i < MATRIX_LIGHT_COLS; i++) {
        gpio_set_pin_input(col_pins[i]);
    }
}

// 驱动单行 (低电平)
static void drive_row(uint8_t row_idx) {
    gpio_set_pin_output(row_pins[row_idx]);
    gpio_write_pin_low(row_pins[row_idx]);
}

// 驱动单列 (高电平)
static void drive_col(uint8_t col_idx) {
    gpio_set_pin_output(col_pins[col_idx]);
    gpio_write_pin_high(col_pins[col_idx]);
}

// -----------------------------------------------------------------------------
// 位翻转函数，用于解决字符左右镜像显示问题
// -----------------------------------------------------------------------------
static uint8_t reverse_bits_in_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4; // 交换半字节
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2; // 交换每2位
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1; // 交换相邻位
    return b;
}

// -----------------------------------------------------------------------------
// 矩阵显示逻辑
// -----------------------------------------------------------------------------

// 刷新矩阵显示
static void refresh_matrix_display(void) {
    for (uint8_t i = 0; i < MATRIX_LIGHT_ROWS; i++) {
        // 计算实际要读取的行索引
        uint8_t actual_row_idx;
        #if MATRIX_LIGHT_FLIP_VERTICAL == 1
            actual_row_idx = (MATRIX_LIGHT_ROWS - 1) - i; // 上下镜像：反转行索引
        #else
            actual_row_idx = i; // 不上下镜像：直接使用当前行索引
        #endif

        // 先将所有引脚置为高阻态，避免鬼影
        set_row_pins_high_z();
        set_col_pins_high_z();

        // 驱动当前行 (低电平)，使用当前循环的行索引
        drive_row(i); // 这里仍使用 i 来驱动物理行

        // 遍历列，根据实际行数据来驱动列
        uint8_t row_data = current_pattern[actual_row_idx]; // 从实际行索引获取数据

        #if MATRIX_LIGHT_FLIP_HORIZONTAL == 1
            row_data = reverse_bits_in_byte(row_data); // 左右镜像：对行数据进行位翻转
        #endif

        for (uint8_t col = 0; col < MATRIX_LIGHT_COLS; col++) {
            if ((row_data >> col) & 0x01) { // 如果该位是1，表示该点亮
                drive_col(col); // 驱动列 (高电平)
            }
        }
        wait_us(100); // 扫描间隔，需要根据实际情况调整，避免闪烁
    }
}

// -----------------------------------------------------------------------------
// 外部接口
// -----------------------------------------------------------------------------

void custom_matrix_light_init(void) {
    // 初始化所有引脚为高阻态
    set_row_pins_high_z();
    set_col_pins_high_z();
    last_action_timer = timer_read(); // 初始化计时器
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    srand(timer_read()); // 为 rand() 函数播种，使下雨效果更随机
    #endif
}

void custom_matrix_light_set_char(char c) {
    // 查找字符点阵数据
    if (c >= ' ' && c <= '~') { // 仅处理可见ASCII字符 (0x20 到 0x7E)
        memcpy(current_pattern, character_patterns[c - ' '], 8);
    } else {
        // 对于无法识别的字符，显示预定义的笑脸图案
        memcpy(current_pattern, SMILEY_FACE_FOR_UNKNOWN, 8); // 使用笑脸图案
    }
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    is_raining = false; // 有新字符显示，停止下雨
    #endif
    last_action_timer = timer_read(); // 重置计时器
}

void custom_matrix_light_set_pattern(const uint8_t pattern[8]) {
    memcpy(current_pattern, pattern, 8);
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    is_raining = false; // 有新图案显示，停止下雨
    #endif
    last_action_timer = timer_read(); // 重置计时器
}

#if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
void custom_matrix_light_start_rain(void) {
    is_raining = true;
    last_rain_update = timer_read();
    // 初始化下雨图案，可以随机生成一些雨滴
    for (uint8_t r = 0; r < 8; r++) {
        for (uint8_t c = 0; c < 8; c++) {
            rain_pattern[r][c] = (rand() % 10 == 0) ? 1 : 0; // 10%的概率出现雨滴
        }
    }
    last_action_timer = timer_read(); // 重置计时器
}
#endif

// 设置指定坐标的灯亮或灭 (true表示亮, false表示灭)
void custom_matrix_light_set_pixels(const matrix_pixel_t* points, size_t num_points, bool state) {
    bool pixel_changed = false; // 标记是否有像素状态被改变

    // 如果 intention 是点亮像素（state 为 true），则首先清空整个矩阵
    // 这将实现“全部刷新只点亮指定的，以前点亮的都灭掉”的效果
    if (state) {
        memset(current_pattern, 0x00, 8); // 将 current_pattern 的所有字节清零
        pixel_changed = true; // 因为清空了模式，所以肯定有变化
    }

    for (size_t i = 0; i < num_points; i++) {
        uint8_t row = points[i].row;
        uint8_t col = points[i].col;

        // 检查行和列是否在有效范围内
        if (row < MATRIX_LIGHT_ROWS && col < MATRIX_LIGHT_COLS) {
            uint8_t old_pattern_row = current_pattern[row]; // 保存旧的行数据

            if (state) {
                // 设置对应的位为1，点亮灯
                current_pattern[row] |= (1 << col);
            } else {
                // 设置对应的位为0，熄灭灯
                current_pattern[row] &= ~(1 << col);
            }

            // 如果当前行的模式发生变化，则标记为有改变
            if (old_pattern_row != current_pattern[row]) {
                pixel_changed = true;
            }
        }
    }

    if (pixel_changed) {
        #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
        is_raining = false;        // 如果有像素被改变，停止下雨效果
        #endif
        last_action_timer = timer_read(); // 重置计时器
    }
}

#if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
void custom_matrix_light_stop_rain(void) {
    is_raining = false;
    memset(current_pattern, 0x00, 8); // 停止下雨后清空显示
}

// 模拟下雨效果：雨滴向下移动，新雨滴从顶部生成
static void update_rain_effect(void) {
    // 每次更新时，将所有雨滴向下移动一行
    for (int8_t r = 7; r >= 0; r--) {
        for (uint8_t c = 0; c < 8; c++) {
            if (r == 0) {
                // 最上面一行随机生成新的雨滴
                rain_pattern[r][c] = (rand() % 10 == 0) ? 1 : 0;
            } else {
                rain_pattern[r][c] = rain_pattern[r - 1][c];
            }
        }
    }

    // 将雨滴模式复制到当前显示模式
    for (uint8_t r = 0; r < 8; r++) {
        current_pattern[r] = 0;
        for (uint8_t c = 0; c < 8; c++) {
            if (rain_pattern[r][c]) {
                current_pattern[r] |= (1 << c);
            }
        }
    }
}
#endif

// 新增函数：关闭所有点阵灯
void custom_matrix_light_clear_all(void) {
    memset(current_pattern, 0x00, 8); // 清空当前显示模式，所有灯熄灭
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    is_raining = false; // 停止下雨效果（如果正在进行）
    #endif
    last_action_timer = timer_read(); // 重置操作计时器
}

void custom_matrix_light_task(void) {
    #if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
    // 根据配置的超时时间，无操作后显示下雨动态效果
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

    // 无论是否下雨，都需要持续刷新显示
    refresh_matrix_display();
}