/*
Copyright (c) 2017 Bryan Hughes <bryan@nebri.us>

This file is part of Raver Lights.

Raver Lights is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Raver Lights is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Raver Lights.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H_
#define CONFIG_H_

/**** You'll probably want to modify these settings ****/

#define LED_COLOR_MODE GRB
#define LED_NUM_PIXELS 300

/**** You *may* want to modify these settings, but it's not likely ****/

#define UPDATE_RATE 33

#ifndef DEFAULT_WIFI_SSID
#define DEFAULT_WIFI_SSID "RaverLights"
#endif
#ifndef DEFAULT_WIFI_PASSPHRASE
#define DEFAULT_WIFI_PASSPHRASE "RaverLights"
#endif

#define DEFAULT_WIFI_PORT 4978
#define SERIAL_BAUDRATE 115200

#define DEFAULT_CHANNEL 0
#define DEFAULT_PRESET 2
#define DEFAULT_BRIGHTNESS 50
#define MIN_BRIGHTNESS 25
#define MAX_BRIGHTNESS 200

/**** You most likely should *not* modify these settings ****/

#define BUTTON_PRESS_ENGAGE_TIME 50
#define BUTTON_HOLD_ENGAGE_TIME 2000

#ifdef RVL_CONTROLLER
#include "./config-controller.hpp"
#endif
#ifdef RVL_HUB
#include "./config-hub.hpp"
#endif
#ifdef RVL_RECEIVER
#include "./config-receiver.hpp"
#endif

#endif // CONFIG_H_
