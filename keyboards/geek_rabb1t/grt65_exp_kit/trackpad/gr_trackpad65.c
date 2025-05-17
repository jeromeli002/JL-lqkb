/* Copyright 2024 Geek-rabb1t (@geek-rabb1t)
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
#include "wait.h"
#include "gr_trackpad65_driver.h"
#include "trackpad/gr_trackpad65_config.h"
#include "trackpad/gr_trackpad65.h"

static trackpad_matrix_config_t trackpad_matrix_config = {0};

void matrix_scan_trackpad(void) {

    if (trackpad_event.type != trackpad_event_none) {
        int row = trackpad_event.num_of_fingers == 3 ?
            trackpad_matrix_config.three_finger_gesture_row:
            trackpad_matrix_config.four_finger_gesture_row;
        action_exec(MAKE_KEYEVENT(row, trackpad_event.type, true));
        wait_ms(10);
        action_exec(MAKE_KEYEVENT(row, trackpad_event.type, false));
        reset_trackpad_event();
    }

    matrix_scan_user();
 }

report_mouse_t pointing_device_task_trackpad(report_mouse_t mouse_report) {
    return pointing_device_task_user(mouse_report);
}


bool process_record_trackpad(uint16_t keycode, keyrecord_t *record) {
    uprintf("cursor_speed: %u\n", gr_trackpad_config.cursor_speed);
    uprintf("cursor_accel: %u\n", gr_trackpad_config.cursor_accel);
    uprintf("scroll_speed: %u\n", gr_trackpad_config.scroll_speed);
    uprintf("tap_sensitivity: %u\n", gr_trackpad_config.tap_sensitivity);
    uprintf("tap: %u\n", gr_trackpad_config.tap);
    uprintf("three_finger_tap: %u\n", gr_trackpad_config.three_finger_tap);
    uprintf("reverse_vertical_scroll: %u\n", gr_trackpad_config.reverse_vertical_scroll);
    uprintf("reverse_horizontal_scroll: %u\n", gr_trackpad_config.reverse_horizontal_scroll);
    uprintf("inertia_cursor: %u\n", gr_trackpad_config.inertia_cursor);
    uprintf("inertia_scroll: %u\n", gr_trackpad_config.inertia_scroll);
    uprintf("move_on_edge: %u\n", gr_trackpad_config.move_on_edge);
    uprintf("scroll_only: %u\n", gr_trackpad_config.scroll_only);
    uprintf("rotate: %u\n\n", gr_trackpad_config.rotate);

    return process_record_user(keycode, record);
}

void keyboard_post_init_trackpad(trackpad_matrix_config_t config) {

    trackpad_matrix_config.two_finger_gesture_row = config.two_finger_gesture_row;
    trackpad_matrix_config.three_finger_gesture_row = config.three_finger_gesture_row;
    trackpad_matrix_config.four_finger_gesture_row = config.four_finger_gesture_row;
    trackpad_matrix_config.configuration_row = config.configuration_row;
    trackpad_matrix_config.configuration_layer = config.configuration_layer;
    trackpad_matrix_config.allow_rotate = config.allow_rotate;

    load_gr_trackpad65_config(
        trackpad_matrix_config.configuration_layer,
        trackpad_matrix_config.configuration_row,
        trackpad_matrix_config.allow_rotate);


    // Customise these values to desired behaviour
    debug_enable = true;
    // debug_matrix = true;
    debug_keyboard = true;
    debug_mouse = true;
}
