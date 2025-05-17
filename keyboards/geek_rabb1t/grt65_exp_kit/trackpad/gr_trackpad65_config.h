/* Copyright 2025 Geek-rabb1t (@geek-rabb1t)
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
#pragma once

#include QMK_KEYBOARD_H

enum GR_TRACKPAD_KEYCODES {
    GRT_SPD_0 = QK_KB_0,
    GRT_SPD_1,
    GRT_SPD_2,
    GRT_SPD_3,
    GRT_SPD_4,
    GRT_SPD_5,
    GRT_SPD_6,
    GRT_SPD_7,
    GRT_SPD_8,
    GRT_SPD_9,

    GRT_ON,
    GRT_OFF,

    GRT_R0,
    GRT_R90,
    GRT_R180,
    GRT_R270,

    HIGH_SPEED,
    LOW_SPEED,
    SCROLL_MODE,

    GRTL_CS,
    GRTL_CA,
    GRTL_SS,
    GRTL_TS,
    GRTL_ET,
    GRTL_ETFT,
    GRTL_RVS,
    GRTL_RHS,
    GRTL_IC,
    GRTL_IS,
    GRTL_MOE,
    GRTL_SO,
    GRTL_R

} gr_trackpad_keycodes_t;

typedef enum {
    ROTATE_0 = GRT_R0,
    ROTATE_90 = GRT_R90,
    ROTATE_180 = GRT_R180,
    ROTATE_270 = GRT_R270
} rotate_modes_t;

typedef struct {
    uint16_t cursor_speed;
    uint16_t cursor_accel;
    uint16_t scroll_speed;
    uint16_t tap_sensitivity;

    bool tap;
    bool three_finger_tap;

    bool reverse_vertical_scroll;
    bool reverse_horizontal_scroll;

    bool inertia_cursor;
    bool inertia_scroll;
    bool move_on_edge;

    bool scroll_only;

    rotate_modes_t rotate;
} gr_trackpad65_config_t;



extern gr_trackpad65_config_t gr_trackpad_config;

void load_gr_trackpad65_config(uint8_t config_layer, uint8_t config_row, bool allow_rotate) ;

