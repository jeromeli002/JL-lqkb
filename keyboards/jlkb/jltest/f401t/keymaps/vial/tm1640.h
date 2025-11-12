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

// 流水灯方向
#ifndef TM1640_RUNNING_DIRECTION
#define TM1640_RUNNING_DIRECTION 0 
#endif

// 【新增默认值】默认亮度
#ifndef TM1640_DEFAULT_BRIGHTNESS
#define TM1640_DEFAULT_BRIGHTNESS 7 // 默认最大亮度
#endif

// -------------------------- 固定配置 --------------------------
// TM1640 亮度命令映射：将 0-7 的配置映射到 0x88-0x8F 
#define TM1640_BRIGHTNESS_COMMAND(level) (0x88 | ((level) & 0x07))

// 使用配置的默认亮度
#define TM1640_DEFAULT_BRIGHTNESS_CMD TM1640_BRIGHTNESS_COMMAND(TM1640_DEFAULT_BRIGHTNESS)


// 【精简】移除旧的 tm1640_brightness_t 枚举，使用 uint8_t 替代
typedef uint8_t tm1640_brightness_t;

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