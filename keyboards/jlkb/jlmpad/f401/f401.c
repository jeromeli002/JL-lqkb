/* Copyright 2021 yushakobo
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

#include "quantum.h"

#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = {
    {
        {NO_LED, NO_LED, NO_LED, NO_LED, NO_LED},
		{18, 17, 16, 15, NO_LED},
		{14, 13, 12, 11, 10},
		{9, 8, 7, 6, 5},
		{4, 3, 2, 1, 0},
    },
    {
        {84, 13}, {28, 13}, {140, 3}, {84, 3}, 
		{140, 24}, {84, 24}, {28, 24}, {196, 13}, {140, 13}, 
		{28, 45}, {140, 35}, {84, 35}, {28, 35}, {196, 26}, 
		{140, 56}, {56, 56}, {196, 48}, {140, 45}, {84, 45}, 
    },
    {
        4, 4, 4, 4, 
		4, 4, 4, 4, 4, 
		4, 4, 4, 4, 4, 
		4, 4, 4, 4, 4,
    }
};
#endif
