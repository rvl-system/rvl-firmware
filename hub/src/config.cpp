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

#include <Arduino.h>
#ifdef HAS_NETWORK
#include <WiFiUdp.h>
#endif
#include "./config.h"

#ifdef HAS_NETWORK
IPAddress SERVER_IP(192, 168, 42, 1);
IPAddress GATEWAY(192, 168, 42, 255);
IPAddress SUBNET(255, 255, 255, 0);
#endif

const char* presetNames[NUM_PRESETS] = {
  "FADE",
  "PULSE",
  "WAVE"
};

const char* presetValueLabels[NUM_PRESETS][NUM_PRESET_VALUES] = {
  { "RATE", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
  { "RATE", "HUE", "SATR", NULL, NULL, NULL, NULL, NULL, NULL, NULL },
  { "RATE", "SPAC", "HUEF", "HUEB", NULL, NULL, NULL, NULL, NULL, NULL }
};

const byte presetValueMin[NUM_PRESETS][NUM_PRESET_VALUES] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 4, 0, 0, 0, 0, 0, 0, 0, 0 }
};

const byte presetValueMax[NUM_PRESETS][NUM_PRESET_VALUES] = {
  { 255, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 255, 255, 255, 0, 0, 0, 0, 0, 0, 0 },
  { 255, 32, 255, 255, 0, 0, 0, 0, 0, 0 }
};

const byte presetValueDefaults[NUM_PRESETS][NUM_PRESET_VALUES] = {
  { 125, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 125, 120, 255, 0, 0, 0, 0, 0, 0, 0 },
  { 85, 16, 120, 160, 0, 0, 0, 0, 0, 0 }
};
