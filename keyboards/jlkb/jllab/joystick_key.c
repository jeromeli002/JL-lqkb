#include "analog.h" // 包含模拟输入相关的头文件（例如 analogReadPin 函数的声明）

// ----------------------------------------------------
// 摇杆映射按键相关配置
// ----------------------------------------------------
static int actuation = 256;          // 触发点：摇杆模拟值偏移中心多少时开始触发按键。
                                     // 模拟值范围通常是 0-1023，中心点约为 512。
                                     // 经过中心校正后，范围变为 -512 到 511。
                                     // 此处 256 表示摇杆需要偏离中心至少 256 个单位（即原始值需大于 512+256=768 或小于 512-256=256）才能被考虑触发。
static int hysteresis = 50;          // 滞后量：用于防止摇杆在触发点附近轻微抖动时造成按键状态反复切换（抖动）。
                                     // 触发： value > actuation + hysteresis (例如 256 + 50 = 306)
                                     // 释放： value < actuation - hysteresis (例如 256 - 50 = 206)

static pin_t joystick_axes_x_pin[] = JOYSTICK_KEY_X_PIN; // X轴（例如左右方向）模拟信号输入的引脚数组。
static pin_t joystick_axes_y_pin[] = JOYSTICK_KEY_Y_PIN; // Y轴（例如上下方向）模拟信号输入的引脚数组。
// 注意：JOYSTICK_KEY_X_PIN 和 JOYSTICK_KEY_Y_PIN 需在其他配置文件中定义为 pin_t 类型的数组或宏。

// ----------------------------------------------------
// 辅助宏和结构体定义
// ----------------------------------------------------

// 计算摇杆数量：通过计算 X 轴引脚数组的大小来确定连接的摇杆数量。
// 注意：确保 JOYSTICK_KEY_X_PIN 是正确的数组定义或宏，并且 X 和 Y 轴的数量一致。
#define NUMBER_OF_JOYSTICKS (sizeof(joystick_axes_x_pin) / sizeof(pin_t)) 

// 定义矩阵位置结构体：用于存储矩阵键盘中一个按键的物理位置（行和列）。
typedef struct {
    uint8_t row; // 行索引
    uint8_t col; // 列索引
} matrix_pos_t;

// 定义四个方向按键在矩阵键盘中的位置（数组）：
// 每个数组的长度应与 NUMBER_OF_JOYSTICKS 相等。
matrix_pos_t joykey_xp[] = JOYSTICK_KEY_PX_POS; // X轴正方向（例如 右/上）按键在矩阵中的位置数组
matrix_pos_t joykey_xn[] = JOYSTICK_KEY_NX_POS; // X轴负方向（例如 左/下）按键在矩阵中的位置数组
matrix_pos_t joykey_yp[] = JOYSTICK_KEY_PY_POS; // Y轴正方向（例如 上/右）按键在矩阵中的位置数组
matrix_pos_t joykey_yn[] = JOYSTICK_KEY_NY_POS; // Y轴负方向（例如 下/左）按键在矩阵中的位置数组
// 注意：JOYSTICK_KEY_P/NX/PY/NY_POS 需在其他配置文件中定义为 matrix_pos_t 类型的数组或宏。

// 存储每个摇杆四个方向按键的当前状态（按下/释放）。
// 数组大小为 4 * 摇杆数量：
// 0: X正, 1: X负, 2: Y正, 3: Y负 (按摇杆索引 i 依次排列：i*4+0, i*4+1, i*4+2, i*4+3)
bool arrows[4 * NUMBER_OF_JOYSTICKS] = {false}; 

// ----------------------------------------------------
// 函数实现
// ----------------------------------------------------

/**
 * @brief 更新摇杆某个方向按键的状态并触发 QMK 按键事件
 * * @param joystick_index 当前摇杆的索引 (0到NUMBER_OF_JOYSTICKS-1)
 * @param value 经过中心校正的摇杆模拟值。对于正向触发，value 约为 0 到 511；对于负向触发，value 也是正数。
 * 这个值代表摇杆偏离中心的程度。
 * @param state 指向当前按键状态的指针 (true: 按下, false: 释放)
 * @param key_pos 指向该方向键在矩阵中位置的数组（例如 joykey_xp）
 */
void update_arrow_state(int joystick_index, int value, bool* state, matrix_pos_t* key_pos) {
    // 1. 获取当前摇杆映射的矩阵位置对应的键码 (Keycode)
    // - biton32(layer_state) 用于获取当前激活的层（Layer）。
    // - dynamic_keymap_get_keycode 根据当前层、行和列来查找对应的键码（KC_A, KC_ESC, MO(1) 等）。
    uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), key_pos[joystick_index].row, key_pos[joystick_index].col);

    // 2. 判断是否满足**按下**和**释放**的条件（包含滞后量 hysteresis）
    bool should_press = value > actuation + hysteresis; // 达到触发点 + 滞后量，准备按下
    bool should_release = value < actuation - hysteresis; // 降到触发点 - 滞后量，准备释放

    if (!(*state) && should_press) {
        // --- 触发按下（当前状态为未按下，且摇杆值达到按下阈值） ---
        *state = true; // 更新状态为已按下
        
        // 构造 keyrecord_t 结构体来模拟按键事件
        keyrecord_t record;
        record.event.key.row = key_pos[joystick_index].row; // 矩阵行
        record.event.key.col = key_pos[joystick_index].col; // 矩阵列
        
        // 键码是 keyrecord_t 的直接成员（用于兼容大多数 QMK 版本）
        record.keycode = keycode; 
        
        record.event.pressed = true; // 按下事件
        record.event.time = timer_read(); // 获取当前时间戳
        
        // 使用 QMK 的核心函数 process_record() 来处理按键事件
        // 这将模拟一次物理按键的按下，确保宏、层操作等特殊功能键能正确触发
        process_record(&record);
        
    } else if (*state && should_release) {
        // --- 触发释放（当前状态为已按下，且摇杆值降到释放阈值） ---
        *state = false; // 更新状态为已释放
        
        // 构造 keyrecord_t 结构体来模拟按键事件
        keyrecord_t record;
        record.event.key.row = key_pos[joystick_index].row; // 矩阵行
        record.event.key.col = key_pos[joystick_index].col; // 矩阵列
        
        // 键码是 keyrecord_t 的直接成员
        record.keycode = keycode; 
        
        record.event.pressed = false; // 释放事件
        record.event.time = timer_read(); // 获取当前时间戳
        
        // 使用 QMK 的核心函数 process_record() 来处理按键事件
        // 这将模拟一次物理按键的释放
        process_record(&record);
    }
}

/**
 * @brief QMK 矩阵扫描函数（主循环中定期调用）
 * * 此函数是 QMK 固件中定期执行的关键函数，用于读取输入并更新键盘状态。
 */
void matrix_scan_kb(void) {
    // 静态数组，用于存储每个摇杆的原始模拟值，确保值在函数调用之间保持。
    static int x_value[NUMBER_OF_JOYSTICKS]; 
    static int y_value[NUMBER_OF_JOYSTICKS]; 

    for (uint8_t i = 0; i < NUMBER_OF_JOYSTICKS; i++) { // 遍历每个摇杆
        // 1. 读取 X/Y 轴模拟值 (通常范围是 0-1023)
        x_value[i] = analogReadPin(joystick_axes_x_pin[i]); // 读取 X 轴引脚的原始模拟值
        y_value[i] = analogReadPin(joystick_axes_y_pin[i]); // 读取 Y 轴引脚的原始模拟值

        // 2. ----------------------------------------------------
        //    X 轴处理
        //    ----------------------------------------------------
        
        // X轴正方向 (例如，右方向)
        // 传入的值为 x_value[i] - 512，得到一个相对于中心 (512) 的偏移量。
        // 当摇杆向正方向推时（例如 > 512），结果为正数。
        update_arrow_state(i, 
                           x_value[i] - 512,        // 摇杆偏移量（正数表示正向偏离）
                           &arrows[i * 4 + 0],      // 对应 X 正方向的状态指针
                           joykey_xp);              // X 正方向按键的矩阵位置

        // X轴负方向 (例如，左方向)
        // 传入的值为 -(x_value[i] - 512)，即 **中心值 - 原始值**。
        // 当摇杆向负方向推时（例如 < 512），结果为正数（偏离中心距离的绝对值）。
        update_arrow_state(i, 
                           -(x_value[i] - 512),     // 摇杆偏移量（负向偏离时转换为正数）
                           &arrows[i * 4 + 1],      // 对应 X 负方向的状态指针
                           joykey_xn);              // X 负方向按键的矩阵位置

        // 3. ----------------------------------------------------
        //    Y 轴处理
        //    ----------------------------------------------------

        // Y轴正方向 (例如，上方向)
        // 传入的值为 y_value[i] - 512，得到相对于中心的偏移量。
        // 当摇杆向正方向推时（例如 > 512），结果为正数。
        update_arrow_state(i, 
                           y_value[i] - 512,        // 摇杆偏移量（正数表示正向偏离）
                           &arrows[i * 4 + 2],      // 对应 Y 正方向的状态指针
                           joykey_yp);              // Y 正方向按键的矩阵位置

        // Y轴负方向 (例如，下方向)
        // 传入的值为 -(y_value[i] - 512)，即 **中心值 - 原始值**。
        // 当摇杆向负方向推时（例如 < 512），结果为正数。
        update_arrow_state(i, 
                           -(y_value[i] - 512),     // 摇杆偏移量（负向偏离时转换为正数）
                           &arrows[i * 4 + 3],      // 对应 Y 负方向的状态指针
                           joykey_yn);              // Y 负方向按键的矩阵位置
    }
}