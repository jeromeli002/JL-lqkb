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
#pragma once

#include "report.h"
#include "pointing_device.h"


typedef enum  {
    trackpad_event_none = -1,
    trackpad_event_swipe_up = 0,
    trackpad_event_swipe_right = 1,
    trackpad_event_swipe_down = 2,
    trackpad_event_swipe_left = 3,
} trackpad_gesture_event_t;


typedef struct {
    trackpad_gesture_event_t type;
    int8_t num_of_fingers;
} trackpad_event_t;

extern trackpad_event_t trackpad_event;

const static int8_t FUTABA_SWIPE_THRESHOLD_PIXEL = 20;
const static uint16_t FUTABA_MAX_GESTURE_ACTIVE_TIME = 1000;
const static uint16_t FUTABA_RETAP_WAITING_TIME = 300;
 // 必ずpressにする閾値
const static uint16_t FUTABA_TAP_STRENGTH_THRESHOLD = 850;
// 弱いタッチでもクリックにする時間の最大値
const static uint16_t FUTABA_MAX_TAP_TIME = 200;

const static int WAIT_TIME_FOR_CURSOR_MOVEMENT = 100;
const static int WAIT_TIME_FOR_MULTI_TAP_CURSOR_MOVEMENT = 200;

#define SCROLL_SCALE_PERCENT 5

void reset_trackpad_event(void);


