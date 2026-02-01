#include "raw_hid.h"

// ========================== 1. RAW HID 指令集 ==========================
bool raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (via_command_bhq(data, length)) {
        return true;
    }
    if (data[0] != 0xAB ) return false;
    
    switch (data[1]) {     
        case 0x00: bootloader_jump(); break;
        case 0x01: eeconfig_init(); wait_ms(200); soft_reset_keyboard(); break;
        case 0x02: soft_reset_keyboard(); break;
     
        default: break;
    }
    return false;
}
