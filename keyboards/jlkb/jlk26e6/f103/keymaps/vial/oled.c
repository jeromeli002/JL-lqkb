
#ifdef OLED_ENABLE
#include "konglong.c"  // render_anime.c render_anime2.c render_anime3.c 分别为三种种大小不同OLED的待机动画，选择加载
// #include <stdio.h>
// [Init Variables] ----------------------------------------------------------//

 #include "logo.c"      //层logo标志
static uint32_t oled_timer          = 0;      // OLED 计时器
bool            master_oled_cleared = false;  // OLED CLEAR 标记
#define OLED_SHOW_STATE_TIMEOUT 30000         // 无操作10秒后激活OLED动画


static void print_status_narrow(void) {
    // Print current mode
  /*  oled_write_ln_P(PSTR("LAYER\n"), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_ln_P(PSTR("00"), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("01"), false);
            break;
        default:
            oled_write_ln_P(PSTR("02\n"), false);
            break;
    }
    oled_write_ln_P(PSTR("\n\n"), false);
    // Print current layer
    oled_write_ln_P(PSTR("mode\n"), false);*/
switch (get_highest_layer(layer_state)) {
        case 0:
 //         oled_set_cursor(1, 0);
            oled_write_raw_P(logo, sizeof(logo));
            break;
        case 1:
            oled_write_raw_P(L1, sizeof(L1));
            break;
        case 2:
            oled_write_raw_P(L2, sizeof(L2));
            break;
        case 3:
            oled_write_raw_P(L3, sizeof(L3));
            break;
        case 4:
            oled_write_raw_P(L4, sizeof(L4));
            break;
        case 5:
            oled_write_raw_P(L5, sizeof(L5));
            break;
        case 6:
            oled_write_raw_P(L6, sizeof(L6));
            break;           
        case 7:
            oled_write_raw_P(L7, sizeof(L7));
            break;          
        case 8:
            oled_write_raw_P(L8, sizeof(L8));
            break;         
        case 9:
            oled_write_raw_P(L9, sizeof(L9));
            break;           
        case 10:
            oled_write_raw_P(L10, sizeof(L10));
            break;           
        case 11:
            oled_write_raw_P(L11, sizeof(L11));
            break;         
        case 12:
            oled_write_raw_P(L12, sizeof(L12));
            break;           
        case 13:
            oled_write_raw_P(L13, sizeof(L13));
            break;           
        case 14:
            oled_write_raw_P(L14, sizeof(L14));
            break;           
        case 15:
            oled_write_raw_P(L15, sizeof(L15));
            break;             
 /*       case 5:
            render_anime();          
            break;
            */
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
    /*
    oled_write_ln_P(PSTR(" "), false);
    oled_write_ln_P(PSTR("LED\n"), false);
    oled_write_P(host_keyboard_led_state().caps_lock ? PSTR("--A--\n") : PSTR("--a--\n"), false);
    oled_write_P(host_keyboard_led_state().num_lock ? PSTR("-Num-\n") : PSTR("-----\n"), false);
    oled_write_P(host_keyboard_led_state().scroll_lock ? PSTR("--S--\n") : PSTR("-- --\n"), false); */
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_180;
    }
    return rotation;
}


bool oled_task_user(void) {
     if (timer_elapsed32(oled_timer) > OLED_SHOW_STATE_TIMEOUT && timer_elapsed32(oled_timer) < 1200000) {
        // 无操作10秒后，OLED_TIMEOUT(60秒默认)前播放动画
        if (!master_oled_cleared) {
            // Clear OLED一次确保动画正确渲染
            oled_clear();
            master_oled_cleared = true;
        }
        render_anime();
        return false;
    } else if (timer_elapsed32(oled_timer) > 1200000) {
        // 无操作60秒后关闭OLED
        oled_off();
        return false;
    } else {
        // 重置Clear OLED标记
        if (master_oled_cleared) {
            oled_on();
            oled_clear();
            master_oled_cleared = false;
        }
        print_status_narrow();

  }return false;
 /*   if (is_keyboard_master()) {
        print_status_narrow();
    } else {
 //     render_logo();
    render_anime();
    }
	return false;*/
}

#endif
