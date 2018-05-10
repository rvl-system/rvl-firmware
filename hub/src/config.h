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

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define WIFI_SSID "RaverLights"
#define WIFI_PASSPHRASE "4&7EWf7p3h222;$+wg9]&4dN,dGh4^@}"

#define SERVER_PORT 3000

#define UPDATE_RATE 33

#define NUM_PRESETS 3
#define NUM_PRESET_VALUES 10

#define MAX_PALETTE_ENTRIES 10

#define DEFAULT_PRESET 2
#define DEFAULT_BRIGHTNESS 25

extern IPAddress SERVER_IP;
extern IPAddress GATEWAY;
extern IPAddress SUBNET;

extern const char* presetNames[NUM_PRESETS];
extern const char* presetValueLabels[NUM_PRESETS][NUM_PRESET_VALUES];
extern const byte presetValueMin[NUM_PRESETS][NUM_PRESET_VALUES];
extern const byte presetValueMax[NUM_PRESETS][NUM_PRESET_VALUES];
extern const byte presetValueDefaults[NUM_PRESETS][NUM_PRESET_VALUES];

extern WiFiUDP udp;

#define DATA_PIN 0
#define CLOCK_PIN 2
#define NUM_PIXELS 120

#define LCD_ADDRESS 0x3C
#define LCD_SDA 4
#define LCD_SCL 5
#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define ACTIVE_CONTRAST 255
#define SHALLOW_IDLE_CONTRAST 16

#define BUTTON_NEXT 12
#define BUTTON_NEXT_ON LOW
#define BUTTON_NEXT_OFF HIGH

#define BUTTON_UP 16
#define BUTTON_UP_ON LOW
#define BUTTON_UP_OFF HIGH

#define BUTTON_DOWN 15
#define BUTTON_DOWN_ON HIGH
#define BUTTON_DOWN_OFF LOW

#define BUTTON_LEFT 14
#define BUTTON_LEFT_ON LOW
#define BUTTON_LEFT_OFF HIGH

#define BUTTON_RIGHT 13
#define BUTTON_RIGHT_ON LOW
#define BUTTON_RIGHT_OFF HIGH

#define BUTTON_PRESS_ENGAGE_TIME 50
#define BUTTON_HOLD_ENGAGE_TIME 2000

#define CLIENT_TIMEOUT 5000

#define CLIENT_SYNC_INTERVAL 2000
#define CLOCK_SYNC_INTERVAL 2000

#endif  // CONFIG_H_
