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

#ifndef CONFIG_ESP8266_H_
#define CONFIG_ESP8266_H_

// No ESP8266 board ships today. This config exists so the esp8266 env compiles
// the ESP8266 transport, which is a submodule shared with other projects. The
// pin is GPIO2 (D4 on a NodeMCU), valid on this chip where the ESP32 configs'
// pin numbers are not
#define LED_DATA_PIN 2

#endif // CONFIG_ESP8266_H_
