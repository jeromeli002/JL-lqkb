#include "quantum.h"
#include "eeprom.h"
#include <string.h>
#include "jlrgb.h"
#include "timer.h" // 必须引入，用于 RGB 休眠计时

// ========================== 1. 数据结构与全局变量 ==========================
remote_rgb_data_t g_remote_rgb_data = {
    .h = 79, .s = 255, .v = 25, .spd = 255
};

typedef struct {
    uint8_t index; uint8_t count; uint8_t h; uint8_t s; uint8_t v;
} led_cfg_t;

typedef struct {
    led_cfg_t caps;
    led_cfg_t num;
    led_cfg_t scrl;
    led_cfg_t layers[16]; 
    uint32_t rgb_timeout; // RGB 自动休眠时间（单位：秒，0为不关闭）
    uint8_t magic; 
} indicator_config_t;

#define INDICATOR_MAGIC 0x8E 
// 【修复点 1】：将 1024 改为 512，避免地址溢出覆盖 QMK 核心 RGB 保存区
#define EEPROM_INDICATOR_ADDR 4096

indicator_config_t g_ind_cfg;

// 用于动态处理 RGB 超时的全局状态变量
static uint32_t custom_last_activity_time = 0;
static bool is_rgb_timeout_sleep = false;

// ========================== 3. 初始化 ==========================
void matrix_init_kb(void) {
    eeprom_read_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    
    if (g_ind_cfg.magic != INDICATOR_MAGIC) {
        // 指示灯默认都不亮，再配置工具改
        g_ind_cfg.caps = (led_cfg_t){0, 1, 0, 0, 0};
        g_ind_cfg.num  = (led_cfg_t){1, 1, 0, 0, 0};
        g_ind_cfg.scrl = (led_cfg_t){2, 1, 0, 0, 0};
        for(uint8_t i=0; i<16; i++) {
            g_ind_cfg.layers[i] = (led_cfg_t){(uint8_t)(27-i), 1, 0, 0, 0};
        }
        g_ind_cfg.rgb_timeout = 180; // 默认 180 秒
        g_ind_cfg.magic = INDICATOR_MAGIC;
        eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    }
    
    custom_last_activity_time = timer_read32(); // 初始化活动时间
    matrix_init_user();
}

void keyboard_post_init_user(void) {
    // 上电强制关闭所有 RGB 灯珠，防止随机亮灯
    // 注意：如果想要开机立刻看到保存的灯效，可以考虑注释掉下面这行
    rgb_matrix_set_color_all(0, 0, 0); 
    
    // 初始化指示灯引脚为输入（高阻态熄灭）
    setPinOutput(B8); writePinLow(B8);
    setPinInput(B1);
    setPinInput(B10);
    setPinInput(B0);
}

// ================= 指示灯及休眠逻辑 (非阻塞实现) =====================
void matrix_scan_user(void) {
    // --- 1. RGB 动态超时休眠逻辑 ---
    if (g_ind_cfg.rgb_timeout > 0) {
        // 判断超过设置的秒数 (乘以1000转换为毫秒)
        if (!is_rgb_timeout_sleep && timer_elapsed32(custom_last_activity_time) > (g_ind_cfg.rgb_timeout * 1000UL)) {
            is_rgb_timeout_sleep = true;
            rgb_matrix_disable_noeeprom(); // 关闭 RGB 矩阵以省电
        }
    } else {
        // 设置为0 (永不休眠) 时的唤醒保护
        if (is_rgb_timeout_sleep) {
            is_rgb_timeout_sleep = false;
            rgb_matrix_enable_noeeprom();
        }
    }

    // --- 2. 纯软件非阻塞 PWM 指示灯亮度控制 ---
    led_t led_state = host_keyboard_led_state();
    bool caps_on = led_state.caps_lock;
    bool num_on  = led_state.num_lock;
    bool scrl_on = led_state.scroll_lock;

    if (caps_on || num_on || scrl_on) {
        // 调节这两个宏来控制亮度：PWM_CYCLE 是总周期，PWM_ON 是亮的时间
        // 扫描率通常在 1000Hz 左右，20次扫描相当于 20ms 左右的周期，1次高电平
        #define PWM_CYCLE 20 
        #define PWM_ON    1  

        static uint8_t pwm_counter = 0;
        pwm_counter++;
        if (pwm_counter >= PWM_CYCLE) {
            pwm_counter = 0;
        }

        if (pwm_counter < PWM_ON) {
            // 点亮（极短时间）
            if (caps_on) { setPinOutput(B1); writePinHigh(B1); }
            if (num_on)  { setPinOutput(B10); writePinHigh(B10); }
            if (scrl_on) { setPinOutput(B0); writePinHigh(B0); }
        } else {
            // 熄灭（高阻态，避免过压或短路影响）
            if (caps_on) { setPinInput(B1); }
            if (num_on)  { setPinInput(B10); }
            if (scrl_on) { setPinInput(B0); }
        }
    } else {
        // 当灯效全关时，确保引脚处于高阻态安全状态
        setPinInput(B1);
        setPinInput(B10);
        setPinInput(B0);
    }
}

// ========================== 5. RGB 指示灯核心逻辑 (最高优先级) ==========================
bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) return false;

    // --- 5.1 处理层灯 (较低优先级) ---
    for (uint8_t i = 0; i < 16; i++) {
        if (IS_LAYER_ON_STATE(layer_state, i)) {
            RGB hsv = hsv_to_rgb((HSV){g_ind_cfg.layers[i].h, g_ind_cfg.layers[i].s, g_ind_cfg.layers[i].v});
            for (uint8_t n = 0; n < g_ind_cfg.layers[i].count; n++) {
                rgb_matrix_set_color(g_ind_cfg.layers[i].index + n, hsv.r, hsv.g, hsv.b);
            }
        }
    }

    // --- 5.2 处理系统指示灯 (最高优先级，覆盖层灯) ---
    led_t led_state = host_keyboard_led_state();

    if (led_state.caps_lock) {
        RGB hsv = hsv_to_rgb((HSV){g_ind_cfg.caps.h, g_ind_cfg.caps.s, g_ind_cfg.caps.v});
        for (uint8_t n = 0; n < g_ind_cfg.caps.count; n++) {
            rgb_matrix_set_color(g_ind_cfg.caps.index + n, hsv.r, hsv.g, hsv.b);
        }
    }

    if (led_state.num_lock) {
        RGB hsv = hsv_to_rgb((HSV){g_ind_cfg.num.h, g_ind_cfg.num.s, g_ind_cfg.num.v});
        for (uint8_t n = 0; n < g_ind_cfg.num.count; n++) {
            rgb_matrix_set_color(g_ind_cfg.num.index + n, hsv.r, hsv.g, hsv.b);
        }
    }

    if (led_state.scroll_lock) {
        RGB hsv = hsv_to_rgb((HSV){g_ind_cfg.scrl.h, g_ind_cfg.scrl.s, g_ind_cfg.scrl.v});
        for (uint8_t n = 0; n < g_ind_cfg.scrl.count; n++) {
            rgb_matrix_set_color(g_ind_cfg.scrl.index + n, hsv.r, hsv.g, hsv.b);
        }
    }

    return true;
}

// ========================== 6. RAW HID 指令集 ==========================
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (length < 2 || data[0] != 0xAB) return;

    switch (data[1]) {
        case 0x00: clear_keyboard(); bootloader_jump(); break;
        case 0x01: eeconfig_init(); wait_ms(200); soft_reset_keyboard(); break;
        case 0x02: soft_reset_keyboard(); break;
        
        case 0x90: 
            g_remote_rgb_data.h = data[2]; g_remote_rgb_data.s = data[3]; 
            g_remote_rgb_data.v = data[4]; g_remote_rgb_data.spd = data[5]; 
            uint8_t copy_len = (length - 6 < MAX_LED_BYTE_COUNT) ? (length - 6) : MAX_LED_BYTE_COUNT;
            memcpy(g_remote_rgb_data.led_bitmap, &data[6], copy_len);
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_color);
            break;
        
        case 0x91: rgb_matrix_mode(RGB_MATRIX_CUSTOM_remote_static_off); break;

        case 0x82: // Caps
            g_ind_cfg.caps.index = data[3]; g_ind_cfg.caps.count = data[4];
            g_ind_cfg.caps.h = data[5]; g_ind_cfg.caps.s = data[6]; g_ind_cfg.caps.v = data[7];
            break;
        case 0x83: // Num
            g_ind_cfg.num.index = data[3]; g_ind_cfg.num.count = data[4];
            g_ind_cfg.num.h = data[5]; g_ind_cfg.num.s = data[6]; g_ind_cfg.num.v = data[7];
            break;
        case 0x84: // Scrl
            g_ind_cfg.scrl.index = data[3]; g_ind_cfg.scrl.count = data[4];
            g_ind_cfg.scrl.h = data[5]; g_ind_cfg.scrl.s = data[6]; g_ind_cfg.scrl.v = data[7];
            break;

        case 0x85: // 设置动态 RGB 休眠时间
            g_ind_cfg.rgb_timeout = ((uint32_t)data[2] << 24) | 
                                    ((uint32_t)data[3] << 16) | 
                                    ((uint32_t)data[4] << 8)  | 
                                     (uint32_t)data[5];
            
            g_ind_cfg.magic = INDICATOR_MAGIC;
            eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
            
            custom_last_activity_time = timer_read32();
            if (is_rgb_timeout_sleep && g_ind_cfg.rgb_timeout > 0) {
                is_rgb_timeout_sleep = false;
                rgb_matrix_enable_noeeprom();
            }
            break;

        case 0x80: // Save
            g_ind_cfg.magic = INDICATOR_MAGIC;
            eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
            break;
        case 0x81: // Layers
            if (data[2] < 16) {
                g_ind_cfg.layers[data[2]].index = data[3];
                g_ind_cfg.layers[data[2]].count = data[4];
                g_ind_cfg.layers[data[2]].h     = data[5];
                g_ind_cfg.layers[data[2]].s     = data[6];
                g_ind_cfg.layers[data[2]].v     = data[7];
            }
            break;
        default: break;
    }
}