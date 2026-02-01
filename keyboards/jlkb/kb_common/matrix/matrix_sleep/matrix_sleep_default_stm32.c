/* Copyright 2025 keymagichorse
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "matrix.h"
#include "quantum.h"
#include "atomic_util.h"
#include "gpio.h"
#include "matrix_sleep.h"

/* matrix wake-up pins */
static const pin_t wakeUpRow_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t wakeUpCol_pins[MATRIX_COLS] = MATRIX_COL_PINS;

/* encoder wake-up pins (from info.json "rotary") */
static const pin_t wakeUpEncA_pins[] = {
    B9, C12, C15, B3, C0, B15, A2
};

static const pin_t wakeUpEncB_pins[] = {
    B8, C13, C14, B4, C1, B14, A1
};

void matrix_sleepConfig(void)
{
    uint8_t i = 0;

#if (DIODE_DIRECTION == COL2ROW)
    /* Set row (low valid), read cols */
    for (i = 0; i < matrix_cols(); i++) {
        if (wakeUpCol_pins[i] == NO_PIN) {
            continue;
        }
        ATOMIC_BLOCK_FORCEON {
            gpio_set_pin_input_high(wakeUpCol_pins[i]);
            palEnableLineEvent(wakeUpCol_pins[i], PAL_EVENT_MODE_RISING_EDGE);
        }
    }

    for (i = 0; i < matrix_rows(); i++) {
        if (wakeUpRow_pins[i] == NO_PIN) {
            continue;
        }
        ATOMIC_BLOCK_FORCEON {
            gpio_set_pin_output(wakeUpRow_pins[i]);
            gpio_write_pin_low(wakeUpRow_pins[i]);
        }
    }

#elif (DIODE_DIRECTION == ROW2COL)
    /* Set col (low valid), read rows */
    for (i = 0; i < matrix_rows(); i++) {
        if (wakeUpRow_pins[i] == NO_PIN) {
            continue;
        }
        ATOMIC_BLOCK_FORCEON {
            gpio_set_pin_input_high(wakeUpRow_pins[i]);
            palEnableLineEvent(wakeUpRow_pins[i], PAL_EVENT_MODE_FALLING_EDGE);
        }
    }

    for (i = 0; i < matrix_cols(); i++) {
        if (wakeUpCol_pins[i] == NO_PIN) {
            continue;
        }
        ATOMIC_BLOCK_FORCEON {
            gpio_set_pin_output(wakeUpCol_pins[i]);
            gpio_write_pin_low(wakeUpCol_pins[i]);
        }
    }
#endif

    /* encoder wake-up config */
    for (i = 0; i < ARRAY_SIZE(wakeUpEncA_pins); i++) {
        if (wakeUpEncA_pins[i] != NO_PIN) {
            ATOMIC_BLOCK_FORCEON {
                gpio_set_pin_input_high(wakeUpEncA_pins[i]);
                palEnableLineEvent(wakeUpEncA_pins[i], PAL_EVENT_MODE_BOTH_EDGES);
            }
        }

        if (wakeUpEncB_pins[i] != NO_PIN) {
            ATOMIC_BLOCK_FORCEON {
                gpio_set_pin_input_high(wakeUpEncB_pins[i]);
                palEnableLineEvent(wakeUpEncB_pins[i], PAL_EVENT_MODE_BOTH_EDGES);
            }
        }
    }
}
