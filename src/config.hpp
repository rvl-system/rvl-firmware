/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

This file is part of RVL Firmware.

RVL Firmware is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RVL Firmware is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RVL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <vector>

#ifndef CONFIG_H_
#define CONFIG_H_

struct StripSegment {
  uint16_t start;
  uint16_t end;
  uint8_t offset;
  bool reverse;
};

/**** You'll probably want to modify these settings ****/

#define LED_COLOR_MODE GRB
#define LED_NUM_PIXELS 300

// For some reason, the screen never comes back on in multicore rendering
// #define SCREEN_SAVER_ENABLED

extern std::vector<StripSegment> segments;

/**** You *may* want to modify these settings, but it's not likely ****/

#define UPDATE_RATE 33

#define DEFAULT_WIFI_PORT 4978
#define SERIAL_BAUDRATE 115200

#define DEFAULT_CHANNEL 0
#define DEFAULT_PRESET 2
#define DEFAULT_BRIGHTNESS 50
#define MIN_BRIGHTNESS 25
#define MAX_BRIGHTNESS 200

#define SCREEN_OFF_TIME 10000

#define CONNECTING_PIXEL_PERIOD 2000
#define CONNECTING_PIXEL_HUE 85

/**** You most likely should *not* modify these settings ****/

#define BUTTON_PRESS_ENGAGE_TIME 50
#define BUTTON_HOLD_ENGAGE_TIME 2000

#ifdef RVL_CONTROLLER
#include "./config-controller.hpp"
#endif
#ifdef RVL_RECEIVER
#include "./config-receiver.hpp"
#endif

#endif // CONFIG_H_
