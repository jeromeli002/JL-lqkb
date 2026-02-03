#include "matrix.h"
#include "quantum.h"
#include "atomic_util.h"
#include "gpio.h"
#include "matrix_sleep.h"
#include "encoder.h"

/* --- 1. 配置读取与极性定义 --- */
#ifdef WAKEUP_PIN
    static const pin_t hw_wakeup_pin = WAKEUP_PIN;
#else
    static const pin_t hw_wakeup_pin = NO_PIN;
#endif

// 极性默认 0 (低有效/上拉)
#ifndef WAKEUP_PIN_POLARITY
    #define WAKEUP_PIN_POLARITY 0
#endif

/* --- 2. 引脚数组定义 --- */
static const pin_t wakeUpRow_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t wakeUpCol_pins[MATRIX_COLS] = MATRIX_COL_PINS;

#ifdef ENCODER_ENABLE
    static const pin_t wakeUpEnc_pins[] = ENCODER_WAKEUP_PINS;
#endif

/**
 * @brief 配置引脚唤醒功能工具函数
 */
static void configure_pin_wakeup(pin_t pin, pal_event_mode_t mode) {
    if (pin != NO_PIN) {
        ATOMIC_BLOCK_FORCEON {
            gpio_set_pin_input_high(pin);
            palEnableLineEvent(pin, mode);
        }
    }
}

void matrix_sleepConfig(void) {
    uint8_t i = 0;

/* --- 3. 矩阵唤醒配置 --- */
#if (DIODE_DIRECTION == COL2ROW)
    for (i = 0; i < matrix_cols(); i++) {
        configure_pin_wakeup(wakeUpCol_pins[i], PAL_EVENT_MODE_FALLING_EDGE);
    }
    for (i = 0; i < matrix_rows(); i++) {
        if (wakeUpRow_pins[i] != NO_PIN) {
            ATOMIC_BLOCK_FORCEON {
                gpio_set_pin_output(wakeUpRow_pins[i]);
                gpio_write_pin_low(wakeUpRow_pins[i]);
            }
        }
    }
#elif (DIODE_DIRECTION == ROW2COL)
    for (i = 0; i < matrix_rows(); i++) {
        configure_pin_wakeup(wakeUpRow_pins[i], PAL_EVENT_MODE_FALLING_EDGE);
    }
    for (i = 0; i < matrix_cols(); i++) {
        if (wakeUpCol_pins[i] != NO_PIN) {
            ATOMIC_BLOCK_FORCEON {
                gpio_set_pin_output(wakeUpCol_pins[i]);
                gpio_write_pin_low(wakeUpCol_pins[i]);
            }
        }
    }
#endif

/* --- 4. 编码器唤醒配置 (修复变量定义及停顿点秒醒问题) --- */
#ifdef ENCODER_ENABLE
    for (i = 0; i < ARRAY_SIZE(wakeUpEnc_pins); i++) {
        // 修正：直接使用数组元素，或定义局部变量
        pin_t current_enc_pin = wakeUpEnc_pins[i]; 
        
        if (current_enc_pin != NO_PIN) {
            ATOMIC_BLOCK_FORCEON {
                // 先设为上拉，检测当前物理状态
                gpio_set_pin_input_high(current_enc_pin);
                
                // 如果当前读取为 0，说明编码器停在了接通 GND 的位置
                if (gpio_read_pin(current_enc_pin) == 0) {
                    // 此时只监听“上升沿”（即拨动滚轮使其断开的瞬间），防止秒醒
                    palEnableLineEvent(current_enc_pin, PAL_EVENT_MODE_RISING_EDGE);
                } else {
                    // 正常没接通状态，双边沿触发（拨动即闭合或断开均唤醒）
                    palEnableLineEvent(current_enc_pin, PAL_EVENT_MODE_BOTH_EDGES);
                }
            }
        }
    }
#endif

/* --- 5. 汇总引脚兼容配置 --- */
    if (hw_wakeup_pin != NO_PIN) {
        ATOMIC_BLOCK_FORCEON {
            palDisableLineEvent(hw_wakeup_pin); 
            if (WAKEUP_PIN_POLARITY == 0) {
                gpio_set_pin_input_high(hw_wakeup_pin);
                palEnableLineEvent(hw_wakeup_pin, PAL_EVENT_MODE_FALLING_EDGE);
            } else {
                gpio_set_pin_input_low(hw_wakeup_pin);
                palEnableLineEvent(hw_wakeup_pin, PAL_EVENT_MODE_RISING_EDGE);
            }
        }
    }
}