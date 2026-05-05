#include "quantum.h"
#include "eeprom.h"
#include <string.h>
#include "jlrgb.h"
#include "wait.h"

// 调节这两个参数调整指示灯亮度
#define LED_BRIGHTNESS 10  // 开启的时间（微秒），越小越暗
#define LED_PERIOD 1000    // 总周期（微秒），1000us = 1ms (1000Hz 刷新率，绝无闪烁)

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
    uint8_t magic; 
} indicator_config_t;

#define INDICATOR_MAGIC 0x8E 
#define EEPROM_INDICATOR_ADDR 1024

indicator_config_t g_ind_cfg;

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
        g_ind_cfg.magic = INDICATOR_MAGIC;
        eeprom_update_block(&g_ind_cfg, (void*)EEPROM_INDICATOR_ADDR, sizeof(g_ind_cfg));
    }
    matrix_init_user();
}

void keyboard_post_init_user(void) {
    // 启用自定义静态模式
    // --- 新增：上电强制关闭所有 RGB 灯珠，防止随机亮灯 ---
    rgb_matrix_set_color_all(0, 0, 0);
    // 在 keyboard_post_init_user 中初始化为输入
    setPinOutput(B8); writePinLow(B8);
    setPinInput(B1);
    setPinInput(B10);
    setPinInput(B0);
//    rgb_matrix_mode(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
}

// ================= 指示灯 =====================
void matrix_scan_user(void) {
    led_t led_state = host_keyboard_led_state();

    // 1. 判断哪些灯该亮
    bool caps_on = led_state.caps_lock;
    bool num_on = led_state.num_lock;
    bool scrl_on = led_state.scroll_lock;

    // 2. 如果有任何一个灯需要亮
    if (caps_on || num_on || scrl_on) {
        // 开启需要亮的引脚
        if (caps_on) { setPinOutput(B1); writePinHigh(B1); }
        if (num_on)  { setPinOutput(B10); writePinHigh(B10); }
        if (scrl_on) { setPinOutput(B0); writePinHigh(B0); }

        // 保持点亮一小会儿 (极其微小的亮度)
        wait_us(LED_BRIGHTNESS);

        // 全部切回输入状态（高阻态熄灭）
        setPinInput(B1);
        setPinInput(B10);
        setPinInput(B0);

        // 剩余时间等待，保证频率稳定
        wait_us(LED_PERIOD - LED_BRIGHTNESS);
    }
}

// ========================== 5. RGB 指示灯核心逻辑 (最高优先级) ==========================
// 此函数在每一帧渲染最后执行，确保指示灯常亮且不被特效覆盖
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
        case 0x00: clear_keyboard();bootloader_jump(); break;
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
