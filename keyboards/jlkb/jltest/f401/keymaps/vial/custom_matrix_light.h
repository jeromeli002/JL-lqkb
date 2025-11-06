#ifndef CUSTOM_MATRIX_LIGHT_H
#define CUSTOM_MATRIX_LIGHT_H

#include QMK_KEYBOARD_H
#include <stddef.h>    

// --------------------------- 用户可配置项 (保留) ---------------------------
#ifndef MATRIX_LIGHT_ROWS
#define MATRIX_LIGHT_ROWS     8
#endif
#ifndef MATRIX_LIGHT_COLS
#define MATRIX_LIGHT_COLS     8
#endif

#ifndef MATRIX_LIGHT_ROW_PINS
#error "MATRIX_LIGHT_ROW_PINS must be defined in config.h"
#endif
#ifndef MATRIX_LIGHT_COL_PINS
#error "MATRIX_LIGHT_COL_PINS must be defined in config.h"
#endif

// --- 新增：初始化效果枚举 ---
typedef enum {
    MATRIX_EFFECT_NONE = 0,         // 无效果（默认，只显示静态像素）
    MATRIX_EFFECT_SCAN_INIT,        // 效果1：逐颗点亮（2秒）
    MATRIX_EFFECT_FLASH_INIT        // 新增效果：闪烁 3 次（代替呼吸）
} matrix_light_effect_t;

// 定义一个结构体来表示一个像素点
typedef struct {
    uint8_t row;
    uint8_t col;
} matrix_pixel_t;

// 初始化矩阵灯驱动（在 keyboard_post_init_user 中调用）
void custom_matrix_light_init(void);

// **新增接口：** 设置并启动初始化效果
void custom_matrix_light_start_effect(matrix_light_effect_t effect);

// **保留：** 设置指定像素点数组的灯亮或灭 (true表示点亮，false表示熄灭)
void custom_matrix_light_set_pixels(const matrix_pixel_t* points, size_t num_points, bool state);

// 在主循环中调用，用于刷新显示和处理动画
void custom_matrix_light_task(void);

// 关闭所有点阵灯
void custom_matrix_light_clear_all(void);

#endif // CUSTOM_MATRIX_LIGHT_H