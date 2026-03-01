#pragma once

#define VIAL_KEYBOARD_UID {0x2E, 0x4D, 0xC1, 0xC9, 0xEF, 0xC3, 0xD6, 0x42}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 2 }

#define OLED_SCROLL_TIMEOUT	 5000 //2000毫秒后开始滚动
#define OLED_SCROLL_TIMEOUT_RIGHT //向右滚动 不设置默认向左滚动
#define OLED_TIMEOUT 0  // 10分钟（毫秒）无操作后关闭屏幕

// [新增] 覆盖默认的 200ms 超时，例如设置为 10s
#define JLOLED_REALTIME_TIMEOUT 2000