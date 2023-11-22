
#include "f401.h"
#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = { {
    {  0,    1,     2},
    { 3,     4,     5},
    { 6,     7,     8}
}, {
    // Key matrix (0 -> 63)
    {0  ,  0}, {16 ,  0}, {32 ,  0},
    {4  , 16}, {24 , 16}, {40 , 16},
    {6  , 32}, {28 , 32}, {44 , 32}, 
} };
#endif