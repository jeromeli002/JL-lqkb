// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <print.h>
#include "trackpad/gr_trackpad65_config.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        MS_BTN1 , MS_BTN3,    MS_BTN2,
        G(KC_TAB), KC_BTN5, G(KC_D), KC_BTN4,
        G(KC_TAB), G(C(KC_RGHT)), G(KC_D), G(C(KC_LEFT)),
        GRTL_CS, GRTL_CA, GRTL_SS, GRTL_TS, GRTL_ET, GRTL_ETFT, GRTL_RVS, GRTL_RHS, GRTL_IC, GRTL_IS, GRTL_MOE, GRTL_SO, GRTL_R,
        GRT_SPD_5, GRT_SPD_5, GRT_SPD_5, GRT_SPD_5, GRT_ON, GRT_ON, GRT_ON, GRT_ON, GRT_ON, GRT_ON, GRT_ON, GRT_ON, GRT_R0
    ),
};

typedef enum  {
    SPEED_MODE_LOW,
    SPEED_MODE_NORMAL,
    SPEED_MODE_HIGH
} speed_mode_t;

static speed_mode_t speed_mode = SPEED_MODE_NORMAL;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case HIGH_SPEED:
      if (record->event.pressed) {
        speed_mode = SPEED_MODE_HIGH;
      } else {
        speed_mode = SPEED_MODE_NORMAL;
      }
      return true;

    case LOW_SPEED:
      if (record->event.pressed) {
        speed_mode = SPEED_MODE_LOW;
      } else {
        speed_mode = SPEED_MODE_NORMAL;
      }
      return true;

    case SCROLL_MODE:
      // TODO 実装する
      return true;

    default:
      return true;
  }
}

int LOW_SPEED_RATIO = 2;
int HIGH_SPEED_RATIO = 2;
#define CONSTRAIN_HID_XY(amt) ((amt) < XY_REPORT_MIN ? XY_REPORT_MIN : ((amt) > XY_REPORT_MAX ? XY_REPORT_MAX : (amt)))

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {

    if (speed_mode == SPEED_MODE_LOW) {
        mouse_report.x = CONSTRAIN_HID_XY(mouse_report.x / LOW_SPEED_RATIO);
        mouse_report.y = CONSTRAIN_HID_XY(mouse_report.y / LOW_SPEED_RATIO);
    }

    if (speed_mode == SPEED_MODE_HIGH) {
        mouse_report.x = CONSTRAIN_HID_XY(mouse_report.x * HIGH_SPEED_RATIO);
        mouse_report.y = CONSTRAIN_HID_XY(mouse_report.y * HIGH_SPEED_RATIO);
    }

    return mouse_report;
}
