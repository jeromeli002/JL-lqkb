
#include "quantum.h"

#ifdef RGB_MATRIX_ENABLE

/* Setting up the LED matrix */
led_config_t g_led_config = { {
  // Key Matrix to LED Index
  { 0, 1, 2, 3 },
  { 4, 5, 6, 7 },
  { 8, 9, 10, 11},
  { 12, 13, 14, 15 },
}, {
  // LED Index to Physical Position
  {  0,0  }, { 12,0  }, { 24,0  }, {36,0  }, 
  {  0,12  }, { 12,12  }, { 24,12  }, {36,12  },
  {  0,24  }, { 12,24  }, { 24,24  }, {36,24  },
  {  0,36  }, { 12,36  }, { 24,36  }, {36,36  }
}, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
} };

#endif