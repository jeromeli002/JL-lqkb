#ifndef TM1640_H
#define TM1640_H

#include "quantum.h"

// -------------------------- 可配置参数（默认值）--------------------------
// 引脚
#ifndef TM1640_DIN_PIN
#define TM1640_DIN_PIN B7
#endif
#ifndef TM1640_SCLK_PIN
#define TM1640_SCLK_PIN B6
#endif

// 尺寸
#ifndef TM1640_ROWS
#define TM1640_ROWS 8
#endif
#ifndef TM1640_COLS
#define TM1640_COLS 8
#endif

// 效果参数
#ifndef TM1640_BLINK_COUNT
#define TM1640_BLINK_COUNT 3
#endif
#ifndef TM1640_BLINK_INTERVAL
#define TM1640_BLINK_INTERVAL 500
#endif
#ifndef TM1640_RUNNING_SPEED
#define TM1640_RUNNING_SPEED 150
#endif

// 【新增默认值】流水灯方向
#ifndef TM1640_RUNNING_DIRECTION
#define TM1640_RUNNING_DIRECTION 0 // 默认纵向
#endif

// -------------------------- 固定配置 --------------------------
// 亮度枚举
typedef enum {
    TM1640_BRIGHTNESS_1_16  = 0x88,
    TM1640_BRIGHTNESS_2_16  = 0x89,
    TM1640_BRIGHTNESS_4_16  = 0x8A,
    TM1640_BRIGHTNESS_10_16 = 0x8B,
    TM1640_BRIGHTNESS_11_16 = 0x8C,
    TM1640_BRIGHTNESS_12_16 = 0x8D,
    TM1640_BRIGHTNESS_13_16 = 0x8E,
    TM1640_BRIGHTNESS_14_16 = 0x8F
} tm1640_brightness_t;

// 灯效类型枚举
typedef enum {
    TM1640_EFFECT_NONE,
    TM1640_EFFECT_BLINK,
    TM1640_EFFECT_RUNNING_LIGHT,
    TM1640_EFFECT_STATIC
} tm1640_effect_type_t;

// 函数声明
void tm1640_init(void);
void tm1640_send_data(const uint8_t *data, tm1640_brightness_t brightness);
void tm1640_display_off(void);
void tm1640_stop_current_effect(void);
void tm1640_start_blink(void);
void tm1640_start_running_light(void);
void tm1640_display_bitmap(const uint8_t *bitmap_data, tm1640_brightness_t brightness);

// 非阻塞式任务函数
void tm1640_task(void);

#endif // TM1640_H