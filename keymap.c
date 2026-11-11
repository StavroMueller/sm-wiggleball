/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
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

// wiggleball for the Ploopy Nano 2 (QMK target: ploopyco/nano_2/rev2_003)

#include QMK_KEYBOARD_H

// The Nano 2's single button is a left click.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( MS_BTN1 )
};

// --- tuning ---------------------------------------------------------------

// Wiggle: this many left/right direction changes in a row toggle scroll mode.
#define WIGGLE_FLIPS 4
// Each direction change must follow the previous one within this time.
#define WIGGLE_TIMEOUT_MS 150
// After a toggle, ignore wiggles for this long.
#define WIGGLE_COOLDOWN_MS 250
// Minimum horizontal movement in one report for it to count as a wiggle stroke.
#define WIGGLE_MIN_X 2

// Sensor counts per scroll wheel tick. Higher = slower scrolling.
#define SCROLL_DIVISOR 64

// ---------------------------------------------------------------------------

static bool     scroll_mode    = false;
static bool     last_dir_right = false;
static uint8_t  flip_count     = 0;
static uint32_t last_flip      = 0;
static uint32_t last_toggle    = 0;
static int32_t  scroll_x       = 0;
static int32_t  scroll_y       = 0;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    int16_t x = mouse_report.x;
    int16_t y = mouse_report.y;

    // wiggle ball detection
    if (timer_elapsed32(last_toggle) > WIGGLE_COOLDOWN_MS) {
        if (timer_elapsed32(last_flip) > WIGGLE_TIMEOUT_MS) {
            flip_count = 0;
        }

        // A stroke counts when it is mostly horizontal and reverses the
        // direction of the previous stroke.
        bool horizontal = abs(x) >= WIGGLE_MIN_X && abs(x) > abs(y);
        if (horizontal && (x > 0) != last_dir_right) {
            last_dir_right = !last_dir_right;
            flip_count++;
            last_flip = timer_read32();
        }

        if (flip_count >= WIGGLE_FLIPS) {
            scroll_mode = !scroll_mode;
            flip_count  = 0;
            scroll_x    = 0;
            scroll_y    = 0;
            last_toggle = timer_read32();
        }
    }

    led_t leds = host_keyboard_led_state();
    if (scroll_mode || leds.caps_lock || leds.num_lock) {
        scroll_x += x;
        scroll_y += y;

        // Ball right = scroll right, ball down = scroll down.
        mouse_report.h = scroll_x / SCROLL_DIVISOR;
        mouse_report.v = -(scroll_y / SCROLL_DIVISOR);
        scroll_x %= SCROLL_DIVISOR;
        scroll_y %= SCROLL_DIVISOR;

        mouse_report.x = 0;
        mouse_report.y = 0;
    }

    return mouse_report;
}
