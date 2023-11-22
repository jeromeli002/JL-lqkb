
#include "f103.h"
#ifdef RGB_MATRIX_ENABLE

#    define XX NO_LED
led_config_t g_led_config = { {
    { 0,     1,     2},
    { 3,     4,     5},
    { 6,     7,     8}
}, {
    // Key matrix (0 -> 63)
    {0  ,  0}, {16 ,  0}, {32 ,  0},
    {4  , 16}, {24 , 16}, {40 , 16},
    {6  , 32}, {28 , 32}, {44 , 32}, 
},

    {   8, 8, 8,     //
        2, 2, 4,     //
        1, 1, 4,     //!! 每个灯是干啥用的

    } };
#endif