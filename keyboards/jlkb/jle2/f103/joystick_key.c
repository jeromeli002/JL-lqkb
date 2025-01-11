#include "analog.h"

//摇杆映射按键相关
static int actuation = 256;       // 触发点，即摇杆偏移多少开始触发按键 (0-511)
static int hysteresis = 50;        // 滞后量，防止抖动，调整此值以达到最佳效果
bool arrows[4];  // 4个反向上下左右

static pin_t joystick_axes_x_pin = JOYSTICK_KEY_X_PIN;  //X轴引脚
static pin_t joystick_axes_y_pin = JOYSTICK_KEY_Y_PIN;  //Y轴引脚

// 定义一个结构体来表示按键在矩阵键盘中的位置
typedef struct {
    uint8_t row; // 行
    uint8_t col; // 列
} 
// 定义四个方向按键在矩阵键盘中的位置
matrix_pos_t;
matrix_pos_t joykey_xp = JOYSTICK_KEY_PX_POS;// 矩阵位置
matrix_pos_t joykey_xn = JOYSTICK_KEY_NX_POS;
matrix_pos_t joykey_yp = JOYSTICK_KEY_PY_POS; 
matrix_pos_t joykey_yn = JOYSTICK_KEY_NY_POS;

void matrix_scan_user(void) {
  // 定义一个结构体，用于存放每个方向的引脚、行列信息、箭头状态以及符号
  struct {
    uint8_t pin;    // 模拟输入引脚
    uint8_t row;    // 键盘矩阵的行
    uint8_t col;    // 键盘矩阵的列
    bool* arrow;  // 指向箭头状态的指针 (true: 按下, false: 松开)
    int8_t sign;   // 符号，1 表示正向 (例如 X+ 或 Y+)，-1 表示负向 (例如 X- 或 Y-)
  } directions[] = {
    {joystick_axes_x_pin, joykey_xp.row, joykey_xp.col, &arrows[0], 1}, // 上 (X 轴正方向)
    {joystick_axes_x_pin, joykey_xn.row, joykey_xn.col, &arrows[1], -1},// 下 (X 轴负方向)
    {joystick_axes_y_pin, joykey_yp.row, joykey_yp.col, &arrows[2], 1}, // 左 (Y 轴正方向)
    {joystick_axes_y_pin, joykey_yn.row, joykey_yn.col, &arrows[3], -1} // 右 (Y 轴负方向)
  };

  // 循环遍历所有方向
  for (int i = 0; i < 4; i++) {
    // 读取模拟输入值并减去中点值 512，得到相对偏移量
    int16_t reading = analogReadPin(directions[i].pin) - 512;
    // 计算阈值上限。sign * (actuation + hysteresis) 实现正负方向阈值的统一计算
    int16_t threshold_high = directions[i].sign * (actuation + hysteresis);
    // 计算阈值下限。sign * (actuation - hysteresis) 实现正负方向阈值的统一计算
    int16_t threshold_low = directions[i].sign * (actuation - hysteresis);

    // 如果当前方向未按下，且读取值超过阈值上限
    if (!*directions[i].arrow && reading > threshold_high) {
      // 设置当前方向为按下状态
      *directions[i].arrow = true;
      // 获取按键码
      uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), directions[i].row, directions[i].col);
      // 注册按键按下
      register_code16(keycode);
    // 如果当前方向已按下，且读取值低于阈值下限
    } else if (*directions[i].arrow && reading < threshold_low) {
      // 设置当前方向为松开状态
      *directions[i].arrow = false;
      // 获取按键码
      uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), directions[i].row, directions[i].col);
      // 注销按键松开
      unregister_code16(keycode);
    }
  }
}