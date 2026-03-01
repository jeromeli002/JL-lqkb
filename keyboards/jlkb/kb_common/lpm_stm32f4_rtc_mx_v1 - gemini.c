/* Copyright 2024 keymagichorse
 *
 * This program is free software: you can redistribute it and/or modify
 * ... (License header remains unchanged) ...
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
#include "bluetooth.h"
#include "matrix_sleep.h"

static uint32_t     lpm_timer_buffer = 0;
static bool         lpm_time_up      = false;
bool is_lpm_via_activity_flag = false;
uint32_t lpm_via_activity_timer = 0;

typedef enum{
    RTC_LIGHT_SLEEP_MODE = 0,
    RTC_DEEP_SLEEP_MODE
} rtc_sleep_mode_enum;
rtc_sleep_mode_enum ret_sleep_mode = RTC_LIGHT_SLEEP_MODE;

#if (DIODE_DIRECTION == COL2ROW)
    static const pin_t wakeUpCol_pins[MATRIX_COLS]   = MATRIX_COL_PINS;
#elif (DIODE_DIRECTION == ROW2COL)
    static const pin_t wakeUpRow_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
#endif

void ws2812power_enabled(void);
void ws2812power_Disabled(void);

void lpm_timer_reset(void) {
    lpm_time_up      = false;
    lpm_timer_buffer = 0;
}

__attribute__((weak)) void lpm_device_power_open(void);
__attribute__((weak)) void lpm_device_power_close(void);

// ==========================================
// STM32F4 专用的 RTC 唤醒定时器设置
// ==========================================
void rtc_wakeup_set(rtc_sleep_mode_enum mode_enmu)
{
    ret_sleep_mode = mode_enmu;

    // 1. 解锁 RTC 写保护
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // 2. 禁用唤醒定时器以进行配置
    RTC->CR &= ~RTC_CR_WUTE;

    // 3. 等待 WUTWF 标志位置位 (允许更新唤醒定时器)
    while((RTC->ISR & RTC_ISR_WUTWF) == 0);

    // 4. 设置唤醒时钟源：选择 RTCCLK / 16 (假设 F4 LSI=32kHz, 则时钟为 2000Hz, 1 tick = 0.5ms)
    RTC->CR &= ~RTC_CR_WUCKSEL; // 清空位域，000 代表 RTCCLK/16

    // 5. 设置重装载值
    switch (mode_enmu)
    {
        case RTC_LIGHT_SLEEP_MODE:
            // 125ms = 250 ticks (250 * 0.5ms = 125ms)
            RTC->WUTR = 250 - 1; 
            break;

        case RTC_DEEP_SLEEP_MODE:
            // 410ms = 820 ticks (820 * 0.5ms = 410ms)
            RTC->WUTR = 820 - 1; 
            break;
    }

    // 6. 重新使能唤醒定时器和唤醒中断
    RTC->CR |= RTC_CR_WUTE | RTC_CR_WUTIE;

    // 7. 开启写保护
    RTC->WPR = 0xFF;
}

void lpm_init(void)
{
    // 禁用调试功能以降低功耗 (F4 同样适用)
    DBGMCU->CR &= ~DBGMCU_CR_DBG_SLEEP;   
    DBGMCU->CR &= ~DBGMCU_CR_DBG_STOP;    
    DBGMCU->CR &= ~DBGMCU_CR_DBG_STANDBY; 

    lpm_timer_reset();

    gpio_write_pin_high(BHQ_INT_PIN);

    // usb
    gpio_set_pin_input(USB_POWER_SENSE_PIN);
    palEnableLineEvent(USB_POWER_SENSE_PIN, PAL_EVENT_MODE_RISING_EDGE);

    lpm_device_power_open();
    rtc_wakeup_set(RTC_LIGHT_SLEEP_MODE);
}

__attribute__((weak)) void lpm_device_power_open(void) {}
__attribute__((weak)) void lpm_device_power_close(void) {}
__attribute__((weak)) void lpm_set_unused_pins_to_input_analog(void) {}

void My_PWR_EnterSTOPMode(void)
{
#if STM32_HSE_ENABLED
    /* Switch to HSI */
    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_SW)) | RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

    /* Set HSE off  */
    RCC->CR &= ~RCC_CR_HSEON;
    while (RCC->CR & RCC_CR_HSERDY);

    palSetLineMode(LPM_STM32_HSE_PIN_IN, PAL_MODE_INPUT_ANALOG); 
    palSetLineMode(LPM_STM32_HSE_PIN_OUT, PAL_MODE_INPUT_ANALOG); 
#endif

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    
    // STM32F4 专有降功耗优化
    PWR->CR &= ~PWR_CR_PDDS;     // 选择进入 STOP 模式 (而非 STANDBY)
    PWR->CR |= PWR_CR_LPDS;      // 调压器进入低功耗模式
    PWR->CR |= PWR_CR_FPDS;      // 在 STOP 模式下 Flash 掉电 (STM32F4 特有，进一步降低功耗)
    PWR->CR |= PWR_CR_CWUF;      // 清除可能的 Wakeup 标志
    
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    
    __WFI();

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
}

void enter_low_power_mode_prepare(void)
{
    if (usb_power_connected()) 
    {
       return;
    }
    lpm_set_unused_pins_to_input_analog();    // 设置没有使用的引脚为模拟输入

    matrix_sleepConfig();

    // ==========================================
    // STM32F4 RTC 唤醒中断打通 (使用 EXTI 22)
    // 注意：不再需要像 F1 那样欺骗 rtcSetAlarm 了
    // ==========================================
    EXTI->PR = EXTI_PR_PR22;         // 清除 EXTI 22 挂起标志
    EXTI->IMR |= EXTI_IMR_MR22;      // 允许 EXTI 22 (RTC Wakeup) 中断
    EXTI->RTSR |= EXTI_RTSR_TR22;    // 设置上升沿触发

    gpio_set_pin_input_low(BHQ_IQR_PIN);
    palEnableLineEvent(BHQ_IQR_PIN, PAL_EVENT_MODE_RISING_EDGE);
    gpio_write_pin_low(BHQ_INT_PIN);

    // usb 插入检测
    gpio_set_pin_input(USB_POWER_SENSE_PIN);
    palEnableLineEvent(USB_POWER_SENSE_PIN, PAL_EVENT_MODE_RISING_EDGE);

    /* Usb unit is actived and running, stop and disconnect first */
    sdStop(&UART_DRIVER);
    palSetLineMode(UART_TX_PIN, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(UART_RX_PIN, PAL_MODE_INPUT_ANALOG);

    usbStop(&USBD1);
    usbDisconnectBus(&USBD1);
    /* USB D+/D- */
    palSetLineMode(A11, PAL_MODE_INPUT_ANALOG);  
    palSetLineMode(A12, PAL_MODE_INPUT_ANALOG); 

    bhq_Disable();
    lpm_device_power_close();    // 外围设备 电源 关闭
    My_PWR_EnterSTOPMode();
}

void exit_low_power_mode_prepare(void)
{
    chSysLock();
        stm32_clock_init();
        halInit();
        stInit();
        timer_init();
    chSysUnlock();

    /* STM32F4 USB D+/D- (PA11/PA12) 重新初始化 */
    /* 使用复用功能 10 (OTG_FS) */
    palSetLineMode(A11, PAL_MODE_ALTERNATE(10));  
    palSetLineMode(A12, PAL_MODE_ALTERNATE(10));  
    
    usb_event_queue_init();
    init_usb_driver(&USBD1);

    matrix_init();

    lpm_timer_reset();
    report_buffer_init();
    bhq_init();     // 包含 uart_init
#if defined (MOUSEKEY_ENABLE)
    mousekey_clear();
#endif
    bhq_common_init();

    lpm_device_power_open();    // 开启外围设备电源
  
    gpio_write_pin_high(BHQ_INT_PIN);
    report_keyboard_t report = {0};
    bluetooth_send_keyboard(&report);   // 发送空包清除按键状态
}

bool lowpower_matrix_task(void) 
{
    bool any_key_pressed = false; 
    uint8_t i = 0;
#if (DIODE_DIRECTION == COL2ROW)
    // Set row(low valid), read cols
    for (i = 0; i < matrix_cols(); i++)
    { 
        if(wakeUpCol_pins[i] == NO_PIN) continue;
        
        if(gpio_read_pin(wakeUpCol_pins[i]) == 0 )
        {
            any_key_pressed = true; 
            return any_key_pressed; 
        }
    }
#elif (DIODE_DIRECTION == ROW2COL)
    // 读取row 有一行是低电平那就唤醒
    for (i = 0; i < matrix_rows(); i++)
    { 
        if(wakeUpRow_pins[i] == NO_PIN) continue;
        
        if(gpio_read_pin(wakeUpRow_pins[i]) == 0 )
        {
            any_key_pressed = true; 
            return any_key_pressed; 
        }
    }
#endif
    return any_key_pressed; 
}

void lpm_via_activity_update(void)
{
    lpm_via_activity_timer = sync_timer_read32();
    is_lpm_via_activity_flag = true;
}

void lpm_task(void)
{
    if (usb_power_connected()) 
    {
       return;
    }

    if(report_buffer_is_empty() == false)
    {
        lpm_time_up = false;
        lpm_timer_buffer = 0;
        return;
    }
    
    if(wireless_get() == WT_STATE_ADV_UNPAIRED || wireless_get() == WT_STATE_ADV_PAIRING)
    {
        lpm_time_up = false;
        lpm_timer_buffer = 0;
        return;
    }
    
    if (is_lpm_via_activity_flag == true)
    {
        if(sync_timer_elapsed32(lpm_via_activity_timer) > (2000 * 60)) 
        {
            lpm_time_up = false;
            lpm_timer_buffer = 0;
            is_lpm_via_activity_flag = false;
            return;
        }
        return;
    }

    if(lpm_time_up == false && lpm_timer_buffer == 0)
    {
        lpm_time_up = true;
        lpm_timer_buffer = sync_timer_read32();
    }

    if (lpm_time_up == true && sync_timer_elapsed32(lpm_timer_buffer) > RUN_MODE_PROCESS_TIME) {
        lpm_time_up = false;
        lpm_timer_buffer = 0;
        enter_low_power_mode_prepare();
        
// ==========================================
// STM32F4 RTC Wakeup 判断逻辑
// ==========================================
        uint8_t temp_cut = 0;
        // STM32F4 的 RTC 唤醒定时器对应 EXTI Line 22
        if (EXTI->PR & EXTI_PR_PR22) {
            EXTI->PR = EXTI_PR_PR22; 
            
            // 清除 RTC 内部的唤醒标志位 (F4 必须手动清，否则无法再次触发)
            RTC->ISR &= ~RTC_ISR_WUTF;
            
            while(1)
            {
                // usb插入时,直接唤醒
                if(usb_power_connected())
                {
                    exit_low_power_mode_prepare();
                    return;
                }
                if(lowpower_matrix_task())
                {
                    break;
                }
                else
                {
                    temp_cut++;
                    if(temp_cut >= 5)
                    {
                        temp_cut = 0;
                        enter_low_power_mode_prepare();
                    }
                }
                wait_us(50);
            }
        } 
        else
        {
            exit_low_power_mode_prepare();
        }
// rtc唤醒逻辑 end 
        exit_low_power_mode_prepare();
    }
}