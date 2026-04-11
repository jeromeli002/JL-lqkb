// rgb_matrix_user.inc

// !!! DO NOT ADD #pragma once !!! //

// ===================================
// Step 1: 声明新的自定义效果 (注册效果名称)
// ===================================
RGB_MATRIX_EFFECT(remote_static_color)  // 接收到的数据来点亮
RGB_MATRIX_EFFECT(remote_static_off)    // 关闭所有 LED
RGB_MATRIX_EFFECT(remote_static_mode0)  // 模式 0

// ==================================================
// Step 2: 在 RGB_MATRIX_CUSTOM_EFFECT_IMPLS 块中包含实现文件
// ==================================================
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "quantum.h"
#include "jlrgb.h" // 引入全局变量声明 (如 g_remote_rgb_data)

// 包含每个独立效果的实现文件
#include "rgb_color.inc"
#include "rgb_off.inc"
#include "rgb_mode0.inc" 

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS