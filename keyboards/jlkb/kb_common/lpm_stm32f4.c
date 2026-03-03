/* Copyright 2024 keymagichorse
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */
#include "quantum.h"
#include "lpm.h"
#include "matrix.h"
#include "gpio.h"
#include "debounce.h"
#include "usb_util.h"
#include <usb_main.h>
#include "bhq.h"
#include "report_buffer.h"
#include "uart.h"
#include "bhq_common.h"
#include "matrix_sleep.h"
#include "bluetooth.h"

#ifdef ENCODER_ENABLE
#    include "encoder.h"
#endif

#if defined(KB_CHECK_BATTERY_ENABLED)
#    include "battery.h"
#endif

static uint32_t     lpm_timer_buffer = 0;
static bool         lpm_time_up      = false;

void lpm_timer_reset(void) {
    lpm_time_up      = false;
    lpm_timer_buffer = 0;
}

__attribute__((weak)) void lpm_device_power_open(void);
__attribute__((weak)) void lpm_device_power_close(void);
__attribute__((weak)) void lpm_set_unused_pins_to_input_analog(void) {}

void lpm_init(void) {
    // 禁用调试模式下的低功耗保持，确保真正进入低功耗
    DBGMCU->CR &= ~(DBGMCU_CR_DBG_SLEEP | DBGMCU_CR_DBG_STOP | DBGMCU_CR_DBG_STANDBY);

    lpm_timer_reset();
    gpio_write_pin_high(BHQ_INT_PIN);

    // USB 插入检测引脚配置
    gpio_set_pin_input(USB_POWER_SENSE_PIN);
    palEnableLineEvent(USB_POWER_SENSE_PIN, PAL_EVENT_MODE_RISING_EDGE);

    lpm_device_power_open();
}

void My_PWR_EnterSTOPMode(void) {
#if STM32_HSE_ENABLED
    /* 切换到 HSI 并关闭 HSE */
    RCC->CFGR = (RCC->CFGR & (~STM32_SW_MASK)) | STM32_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW_HSI << 2));

    RCC->CR &= ~RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY));

    palSetLineMode(LPM_STM32_HSE_PIN_IN, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(LPM_STM32_HSE_PIN_OUT, PAL_MODE_INPUT_ANALOG); 
#endif

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    // 使用低功耗电压调节器以进一步省电
    PWR->CR |= (PWR_CR_MRLVDS | PWR_CR_LPLVDS | PWR_CR_FPDS | PWR_CR_LPDS);
    
    __WFI(); 

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
}

// 辅助函数：配置编码器引脚唤醒
void setup_encoder_wakeup(pin_t pin) {
    if (pin == NO_PIN) return;
    
    // 强制开启内部上拉，因为 C 接地且无外部上拉
    palSetLineMode(pin, PAL_MODE_INPUT_PULLUP);
    
    // 延迟一小会儿让电平稳定
    wait_us(10);
    
    /* * 使用双边沿触发唤醒。
     * 无论编码器停留在接通(Low)还是断开(High)状态，
     * 只要旋钮转动，电平必然发生跳变，从而触发 EXTI 唤醒。
     */
    palEnableLineEvent(pin, PAL_EVENT_MODE_BOTH_EDGES);
}

void enter_low_power_mode_prepare(void) {
    if (usb_power_connected()) return;

    lpm_set_unused_pins_to_input_analog(); 

#if defined(KB_CHECK_BATTERY_ENABLED)
    battery_disable_read();
#endif

    // 1. 矩阵键盘休眠配置
    matrix_sleepConfig();

    // 2. 编码器唤醒配置 (自动忽略未定义的引脚)
#if defined(ENCODER_ENABLE)
#   if defined(ENCODERS_PAD_A) && defined(ENCODERS_PAD_B)
    static const pin_t enc_a[] = ENCODERS_PAD_A;
    static const pin_t enc_b[] = ENCODERS_PAD_B;

    for (uint8_t i = 0; i < (sizeof(enc_a) / sizeof(pin_t)); i++) {
        setup_encoder_wakeup(enc_a[i]);
    }
    for (uint8_t i = 0; i < (sizeof(enc_b) / sizeof(pin_t)); i++) {
        setup_encoder_wakeup(enc_b[i]);
    }
#   endif
#endif

    // 3. 其他唤醒源
    gpio_set_pin_input_low(BHQ_IQR_PIN);
    palEnableLineEvent(BHQ_IQR_PIN, PAL_EVENT_MODE_RISING_EDGE);
    gpio_set_pin_input(BHQ_INT_PIN);

    gpio_set_pin_input(USB_POWER_SENSE_PIN);
    palEnableLineEvent(USB_POWER_SENSE_PIN, PAL_EVENT_MODE_RISING_EDGE);

    // 4. 关闭通信接口
    sdStop(&UART_DRIVER);
    palSetLineMode(UART_TX_PIN, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(UART_RX_PIN, PAL_MODE_INPUT_ANALOG);

    usbStop(&USBD1);
    usbDisconnectBus(&USBD1);

    bhq_Disable();
    lpm_device_power_close(); 
    
    // 进入休眠
    My_PWR_EnterSTOPMode();

    // ================= 唤醒后恢复 =================

    chSysLock();
        stm32_clock_init();
        halInit();
        stInit();
        timer_init();
    chSysUnlock();

    /* 恢复 USB 引脚 */
    palSetLineMode(A11, PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_FLOATING | PAL_MODE_ALTERNATE(10U));
    palSetLineMode(A12, PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_FLOATING | PAL_MODE_ALTERNATE(10U));
    
    usb_event_queue_init();
    init_usb_driver(&USBD1);

    matrix_init();
#ifdef ENCODER_ENABLE
    encoder_init(); 
#endif

    lpm_timer_reset();
    report_buffer_init();
    bhq_init();     
#if defined (MOUSEKEY_ENABLE)
    mousekey_clear();
#endif
    bhq_common_init();
#if defined(KB_CHECK_BATTERY_ENABLED)
    battery_enable_read();
#endif
    lpm_device_power_open();  
  
    gpio_write_pin_high(BHQ_INT_PIN);
    report_keyboard_t report = {0};
    bluetooth_send_keyboard(&report); 
}

void lpm_via_activity_update(void) {}

void lpm_task(void) {
    if (usb_power_connected()) return;

    if(report_buffer_is_empty() == false) {
        lpm_time_up = false;
        lpm_timer_buffer = 0;
        return;
    }

    if(lpm_time_up == false && lpm_timer_buffer == 0) {
        lpm_time_up = true;
        lpm_timer_buffer = sync_timer_read32();
    }

    if (lpm_time_up == true && sync_timer_elapsed32(lpm_timer_buffer) > RUN_MODE_PROCESS_TIME) {
        lpm_time_up = false;
        lpm_timer_buffer = 0;
        enter_low_power_mode_prepare();
    }
}