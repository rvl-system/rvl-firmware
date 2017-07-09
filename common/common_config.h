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

#ifndef COMMON_CONFIG_HEADER
#define COMMON_CONFIG_HEADER

#include <ESP8266WiFi.h>

#define WIFI_SSID "RaverLights"
#define WIFI_PASSPHRASE "4&7EWf7p3h222;$+wg9]&4dN,dGh4^@}"

#define SERVER_PORT 3000

#define UPDATE_RATE 33

#define NUM_PRESETS 2
#define NUM_PRESET_VALUES 10

extern IPAddress SERVER_IP;
extern IPAddress GATEWAY;
extern IPAddress SUBNET;

#endif
