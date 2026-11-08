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

// The strip buffers' size, and the pixel count until one is configured
#define LED_MAX_PIXELS 300
#define DEFAULT_COLOR_ORDER rvl::ColorOrder::GRB
// Of 255. Full was too bright to look at
#define DEFAULT_STATUS_LED_BRIGHTNESS 60
// In mA: a micro USB feed, rated 1.8 A, with a margin
#define DEFAULT_CURRENT_LIMIT 1500

#define SCREEN_SAVER_ENABLED

extern std::vector<StripSegment> segments;

/**** You *may* want to modify these settings, but it's not likely ****/

#define SERIAL_BAUDRATE 115200

#define DEFAULT_CHANNEL 0
#define DEFAULT_PRESET 2
#define DEFAULT_BRIGHTNESS 50
#define MIN_BRIGHTNESS 25
#define MAX_BRIGHTNESS 200

#define SCREEN_OFF_TIME 10000

#define CONNECTING_PIXEL_PERIOD 2000

/**** You most likely should *not* modify these settings ****/

#define BUTTON_PRESS_ENGAGE_TIME 50
#define BUTTON_HOLD_ENGAGE_TIME 2000

// FastLED's power model is in mW at 5 V, so volts times mA gives its mW only at
// this voltage
#define LED_VOLTS 5

#ifdef RVL_CONTROLLER
#include "./config-controller.hpp"
#endif
#ifdef RVL_RECEIVER
#include "./config-receiver.hpp"
#endif
#ifdef RVL_MINI
#include "./config-mini.hpp"
#endif
#ifdef RVL_ESP8266
#include "./config-esp8266.hpp"
#endif

#endif // CONFIG_H_
