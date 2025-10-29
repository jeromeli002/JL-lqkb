#ifndef CUSTOM_MATRIX_LIGHT_H
#define CUSTOM_MATRIX_LIGHT_H

#include QMK_KEYBOARD_H // 包含QMK常用的宏和类型

// --------------------------- 用户可配置项 ---------------------------
// 在config.h中定义这些宏，方便用户配置
// #define MATRIX_LIGHT_ROWS     8  // LED矩阵的行数
// #define MATRIX_LIGHT_COLS     8  // LED矩阵的列数
// #define MATRIX_LIGHT_ROW_PINS { GP0, GP1, GP2, GP3, GP4, GP5, GP6, GP7 } // 实际行引脚
// #define MATRIX_LIGHT_COL_PINS { GP8, GP9, GP10, GP11, GP12, GP13, GP14, GP15 } // 实际列引脚
// --------------------------------------------------------------------

// 新增：如果未在config.h中定义，则使用默认值 8x8
#ifndef MATRIX_LIGHT_ROWS
#define MATRIX_LIGHT_ROWS     8
#endif
#ifndef MATRIX_LIGHT_COLS
#define MATRIX_LIGHT_COLS     8
#endif

// 强制检查引脚定义
#ifndef MATRIX_LIGHT_ROW_PINS
#error "MATRIX_LIGHT_ROW_PINS must be defined in config.h"
#endif
#ifndef MATRIX_LIGHT_COL_PINS
#error "MATRIX_LIGHT_COL_PINS must be defined in config.h"
#endif

// 默认值：如果未在config.h中定义，则使用这些值
#ifndef ENABLE_MATRIX_LIGHT_RAIN_EFFECT
#define ENABLE_MATRIX_LIGHT_RAIN_EFFECT 1 // 默认开启下雨效果
#endif

#if ENABLE_MATRIX_LIGHT_RAIN_EFFECT
#ifndef MATRIX_LIGHT_RAIN_TIMEOUT_MS
#define MATRIX_LIGHT_RAIN_TIMEOUT_MS 5000 // 默认无操作5秒后开启下雨动画
#endif
#endif

// --- 矩阵旋转配置 ---
typedef enum {
    MATRIX_LIGHT_ROTATION_NONE = 0,   // 不旋转 (默认)
    MATRIX_LIGHT_ROTATION_90   = 90,  // 顺时针旋转90度
    MATRIX_LIGHT_ROTATION_180  = 180, // 顺时针旋转180度
    MATRIX_LIGHT_ROTATION_270  = 270  // 顺时针旋转270度
} matrix_light_rotation_t;

#ifndef MATRIX_LIGHT_ROTATION
#define MATRIX_LIGHT_ROTATION MATRIX_LIGHT_ROTATION_NONE // 默认不旋转
#endif
// --------------------------

// 初始化矩阵灯驱动
void custom_matrix_light_init(void);

// 设置要显示的字符（8x8点阵数据，会显示在矩阵的左上角）
void custom_matrix_light_set_char(char c);

// 设置自定义的8x8点阵数据（会显示在矩阵的左上角）
void custom_matrix_light_set_pattern(const uint8_t pattern[8]); // 这里的 pattern 仍然是 8字节

// 开启下雨动态效果 (会填充整个矩阵，无论大小)
void custom_matrix_light_start_rain(void);

// 停止下雨动态效果
void custom_matrix_light_stop_rain(void);

// 在主循环中调用，用于刷新显示和处理动画
void custom_matrix_light_task(void);

// 设置指定坐标的灯亮或灭 (true表示亮, false表示灭)
// 定义一个结构体来表示一个像素点
typedef struct {
    uint8_t row;
    uint8_t col;
} matrix_pixel_t;
// 设置指定像素点数组的灯亮或灭 (true表示亮, false表示灭)
// points: 包含要操作的像素点的数组
// num_points: 数组中像素点的数量
// state: true表示点亮，false表示熄灭
void custom_matrix_light_set_pixels(const matrix_pixel_t* points, size_t num_points, bool state);

// 关闭所有点阵灯
void custom_matrix_light_clear_all(void);

#endif // CUSTOM_MATRIX_LIGHT_H