/*
 * Copyright 2025 Geek-rabb1t (@geek-rabb1t)
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

#include "gr_trackpad65_idle_strategy.h"
#include "gr_trackpad65_strategy_helper.h"
#include "../gr_trackpad65_driver.h"
#include "../gr_trackpad65_config.h"
#include "timer.h"
#include <stdlib.h>

extern trackpad_gesture_handle_state_t gesture_handle_state;

trackpad_state_t update_move_state(trackpad_base_data_t *trackpad_data) {
    touch_state_t touch_state = get_touch_state(trackpad_data);

    if (touch_state == touch_state_none) {
        return trackpad_state_idle;
    }

    return trackpad_state_move;
}


report_mouse_t move_strategy(trackpad_base_data_t *trackpad_data) {
    report_mouse_t temp_report = {0};
    if (trackpad_data->num_of_fingers  >= 2) {

        int scroll_dir_x = (gr_trackpad_config.reverse_horizontal_scroll) ? -1 : 1;
        int scroll_dir_y = (gr_trackpad_config.reverse_vertical_scroll  ) ? -1 : 1;

        gesture_handle_state.scroll_rest.x += trackpad_data->pos.x * SCROLL_SCALE_PERCENT;
        gesture_handle_state.scroll_rest.y += trackpad_data->pos.y * SCROLL_SCALE_PERCENT;
        int scroll_x = gesture_handle_state.scroll_rest.x / 100;
        int scroll_y = gesture_handle_state.scroll_rest.y / 100;
        gesture_handle_state.scroll_rest.x -= scroll_x * 100;
        gesture_handle_state.scroll_rest.y -= scroll_y * 100;

        // Restrict scroll direction.
        if (gesture_handle_state.scroll_direction == scroll_direction_tbd) {
            if (scroll_x == 0 && scroll_y == 0) {
                return temp_report;
            }
            if (abs(scroll_y) > abs(scroll_x) * 2) {
                gesture_handle_state.scroll_direction = scroll_direction_vertical;
            } else if (abs(scroll_x) > abs(scroll_y) *2) {
                gesture_handle_state.scroll_direction = scroll_direction_horizontal;
            } else {
                gesture_handle_state.scroll_direction = scroll_direction_both;
            }

            return temp_report;
        }

        // Remove restrictions on scroll direction.
        if (abs(trackpad_data->pos.y) > abs(trackpad_data->pos.x) * 2  && abs(trackpad_data->prev_pos.y) > abs(trackpad_data->prev_pos.x) * 2) {
            if (gesture_handle_state.scroll_direction == scroll_direction_horizontal) {
                gesture_handle_state.scroll_direction = scroll_direction_both;
            }

        } else if (abs(trackpad_data->pos.x) > abs(trackpad_data->pos.y) * 2  && abs(trackpad_data->prev_pos.x) > abs(trackpad_data->prev_pos.y) * 2) {
            if (gesture_handle_state.scroll_direction == scroll_direction_vertical) {
                gesture_handle_state.scroll_direction = scroll_direction_both;
            }
        }

        if (gesture_handle_state.scroll_direction == scroll_direction_vertical || gesture_handle_state.scroll_direction == scroll_direction_both) {
            temp_report.v = CONSTRAIN_HID(scroll_y * scroll_dir_y);
        }
        if (gesture_handle_state.scroll_direction == scroll_direction_horizontal || gesture_handle_state.scroll_direction == scroll_direction_both) {
            temp_report.h = CONSTRAIN_HID(scroll_x * scroll_dir_x);
        }


    } else {
        gesture_handle_state.scroll_direction = scroll_direction_tbd;
        temp_report.x = trackpad_data->mouse_report_x;
        temp_report.y = trackpad_data->mouse_report_y;
    }

    return temp_report;
}
