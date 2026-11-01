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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "./config.hpp"
#include <Arduino.h>
#include <rvl.hpp>

namespace Settings {

void init();
void loop();

char* getWiFiSSID();
void setWiFiSSID(const char* ssid);

char* getWiFiPassphrase();
void setWiFiPassphrase(const char* passphrase);

// A set is written once the value has been stable for a second, so a flash
// write never sits in the path that answers a packet, and a get sees a pending
// value first. A string is at most NICKNAME_LENGTH characters
uint8_t getSetting(const char* key, uint8_t defaultValue);
void setSetting(const char* key, uint8_t value);
uint16_t getSetting16(const char* key, uint16_t defaultValue);
void setSetting16(const char* key, uint16_t value);
void getStringSetting(const char* key, char* buffer, size_t size);
void setStringSetting(const char* key, const char* value);

} // namespace Settings

#endif // SETTINGS_H_
