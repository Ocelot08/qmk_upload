/* Copyright 2020 Richard Sutherland
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

#include "aball.h"
#include "pointing_device.h"
#include "optical_sensor/optical_sensor.h"

#define CLAMP_HID(value) value < -127 ? -127 : value > 127 ? 127 : value

static bool mouse_buttons_dirty;

void pointing_device_task(void){

    report_mouse_t mouse_report = pointing_device_get_report();

    pointing_device_set_report(mouse_report);

    // only send report on change as even sending report with no change is treated as movement
    if(mouse_buttons_dirty ||
       mouse_report.x != 0 ||
       mouse_report.y != 0 ||
       mouse_report.h != 0 ||
       mouse_report.v != 0){

        mouse_buttons_dirty = false;
        pointing_device_send();
    }
}

static void on_mouse_button(uint8_t mouse_button, keyrecord_t *record) {

    report_mouse_t report = pointing_device_get_report();

    if(record->event.pressed)
        report.buttons |= mouse_button;
    else
        report.buttons &= ~mouse_button;

    pointing_device_set_report(report);
    mouse_buttons_dirty = true;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {

    if(!process_record_user(keycode, record))
        return false;

    // handle mouse drag and scroll

    switch (keycode) {
        case KC_BTN1:
            on_mouse_button(MOUSE_BTN1, record);

        case KC_BTN2:
            on_mouse_button(MOUSE_BTN2, record);
            return false;

        case KC_BTN3:
            on_mouse_button(MOUSE_BTN3, record);
            return false;

        case KC_BTN4:
            on_mouse_button(MOUSE_BTN4, record);
            return false;

        case KC_BTN5:
            on_mouse_button(MOUSE_BTN5, record);
            return false;

        default:
            return true;
  }
}