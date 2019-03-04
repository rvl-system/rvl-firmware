/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

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
#include <ESP8266WiFi.h>
#include <RaverLightsMessaging.h>
#include "./arduino_platform.h"
#include "./state.h"
#include "./codes.h"
#include "./logging.h"
#include "./event.h"

namespace ArduinoPlatform {

SerialLogging logging(RVLLogLevel::Debug);
ArduinoPlatform platform;

SerialLogging::SerialLogging(RVLLogLevel newLevel) {
  this->level = newLevel;
  Serial.begin(115200);
}

RVLLogLevel SerialLogging::getLogLevel() {
  return this->level;
}

void SerialLogging::print(const char s) {
  Serial.print(s);
}

void SerialLogging::print(const char *s) {
  Serial.print(s);
}

void SerialLogging::println() {
  Serial.println();
}

void SerialLogging::println(const char s) {
  Serial.println(s);
}

void SerialLogging::println(const char *s) {
  Serial.println(s);
}

void ArduinoPlatform::setDeviceId(uint16_t newDeviceId) {
  this->deviceId = newDeviceId;
}

uint32_t ArduinoPlatform::getLocalTime() {
  return millis();
}

uint32_t ArduinoPlatform::getClockOffset() {
  return this->clockOffset;
}
void ArduinoPlatform::setClockOffset(uint32_t newOffset) {
  this->clockOffset = newOffset;
}

RVLDeviceMode ArduinoPlatform::getDeviceMode() {
  return this->deviceMode;
}

void ArduinoPlatform::setDeviceMode(RVLDeviceMode newMode) {
  if (this->deviceMode == newMode) {
    return;
  }
  switch (newMode) {
    case RVLDeviceMode::Controller:
      Logging::info("Changing to controller mode");
      this->deviceMode = newMode;
      Event::emit(Codes::EventType::ModeChange);
      break;
    case RVLDeviceMode::Receiver:
      Logging::info("Changing to receiver mode");
      this->deviceMode = newMode;
      Event::emit(Codes::EventType::ModeChange);
      break;
    default:
      Logging::error("Tried to set unknown mode %d", static_cast<uint8_t>(newMode));
  }
}

uint16_t ArduinoPlatform::getDeviceId() {
  return WiFi.localIP()[3];
}

RVLWaveSettings* ArduinoPlatform::getWaveSettings() {
  return &waveSettings;
}

void ArduinoPlatform::setWaveSettings(RVLWaveSettings* newWaveSettings) {
  memcpy(&(this->waveSettings), newWaveSettings, sizeof(RVLWaveSettings));
  if (State::isWifiConnected()) {
    this->onWaveSettingsUpdated();
  }
  Event::emit(Codes::EventType::AnimationChange);
}

}  // namespace ArduinoPlatform
