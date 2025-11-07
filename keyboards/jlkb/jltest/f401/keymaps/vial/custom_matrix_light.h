#ifndef CUSTOM_MATRIX_LIGHT_H
#define CUSTOM_MATRIX_LIGHT_H

#include QMK_KEYBOARD_H
#include <stddef.h>    

// --------------------------- 用户可配置项 ---------------------------
// 矩阵行列数（默认8x8，可在config.h覆盖）
#ifndef MATRIX_LIGHT_ROWS
#define MATRIX_LIGHT_ROWS     8
#endif
#ifndef MATRIX_LIGHT_COLS
#define MATRIX_LIGHT_COLS     8
#endif

// 行列引脚（必须在config.h中定义）
#ifndef MATRIX_LIGHT_ROW_PINS
#error "MATRIX_LIGHT_ROW_PINS must be defined in config.h"
#endif
#ifndef MATRIX_LIGHT_COL_PINS
#error "MATRIX_LIGHT_COL_PINS must be defined in config.h"
#endif

// 电平配置（C2R=COL高/ROW低；R2C=COL低/ROW高，默认C2R）
#ifndef MATRIX_LIGHT_LEVEL_CONFIG
#define MATRIX_LIGHT_LEVEL_CONFIG  C2R
#endif
#if !defined(MATRIX_LIGHT_LEVEL_CONFIG) || (MATRIX_LIGHT_LEVEL_CONFIG != C2R && MATRIX_LIGHT_LEVEL_CONFIG != R2C)
#error "MATRIX_LIGHT_LEVEL_CONFIG must be C2R or R2C"
#endif

// 默认亮度（0-100，默认100）
#ifndef MATRIX_LIGHT_DEFAULT_BRIGHTNESS
#define MATRIX_LIGHT_DEFAULT_BRIGHTNESS  100
#endif
#if MATRIX_LIGHT_DEFAULT_BRIGHTNESS > 100 || MATRIX_LIGHT_DEFAULT_BRIGHTNESS < 0
#error "MATRIX_LIGHT_DEFAULT_BRIGHTNESS must be between 0 and 100"
#endif

// 闪烁效果次数（≥1，默认3次）
#ifndef MATRIX_LIGHT_FLASH_COUNT
#define MATRIX_LIGHT_FLASH_COUNT  3
#endif
#if MATRIX_LIGHT_FLASH_COUNT < 1
#error "MATRIX_LIGHT_FLASH_COUNT must be ≥ 1"
#endif

// 是否启用特殊像素优化（最后一行第一列，默认启用）
#ifndef MATRIX_LIGHT_ENABLE_SPECIAL_PIXEL
#define MATRIX_LIGHT_ENABLE_SPECIAL_PIXEL  1
#endif
#if MATRIX_LIGHT_ENABLE_SPECIAL_PIXEL != 0 && MATRIX_LIGHT_ENABLE_SPECIAL_PIXEL != 1
#error "MATRIX_LIGHT_ENABLE_SPECIAL_PIXEL must be 0 (disable) or 1 (enable)"
#endif

// --------------------------- 枚举和结构体 ---------------------------
typedef enum {
    MATRIX_EFFECT_NONE = 0,         // 无效果（默认）
    MATRIX_EFFECT_SCAN_INIT,        // 逐颗点亮（2秒）
    MATRIX_EFFECT_FLASH_INIT        // 闪烁效果
} matrix_light_effect_t;

typedef struct {
    uint8_t row;
    uint8_t col;
} matrix_pixel_t;

// --------------------------- 外部接口 ---------------------------
void custom_matrix_light_init(void);
void custom_matrix_light_start_effect(matrix_light_effect_t effect);
void custom_matrix_light_set_pixels(const matrix_pixel_t* points, size_t num_points, bool state);
void custom_matrix_light_task(void);
void custom_matrix_light_clear_all(void);

#endif // CUSTOM_MATRIX_LIGHT_H