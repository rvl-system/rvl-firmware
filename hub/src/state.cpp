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
#include "./state.h"
#include "./codes.h"
#include "./event.h"

namespace State {

uint8_t brightness = DEFAULT_BRIGHTNESS;
bool wifiConnected = false;
RVLLogging* logger;
RVLWaveSettings* waveSettings;

void onWaveSettingsUpdated(RVLWaveSettings* settings) {
  waveSettings = settings;
  Event::emit(Codes::EventType::AnimationChange);
}

void onConnectionStateChanged(bool connected) {
  wifiConnected = connected;
  Event::emit(Codes::EventType::ConnectedStateChange);
}

void onModeChanged(RVLDeviceMode newMode) {
  Event::emit(Codes::EventType::ModeChange);
}

void init() {
  RVLESPSetMode(RVLDeviceMode::Receiver);
  State::getLogger()->info("State initialized");
  RVLESPOnWaveSettingsUpdate(onWaveSettingsUpdated);
  RVLESPOnConnectionStateChanged(onConnectionStateChanged);
  RVLESPOnModeChanged(onModeChanged);
}

void loop() {
}

uint8_t getBrightness() {
  return brightness;
}

void setBrightness(uint8_t newBrightness) {
  brightness = newBrightness;
}

bool isWifiConnected() {
  return wifiConnected;
}

void setWifiConnectedState(bool connected) {
  wifiConnected = connected;
  Event::emit(Codes::EventType::ConnectedStateChange);
}

RVLLogging* getLogger() {
  return logger;
}

void setLogger(RVLLogging* newLogger) {
  logger = newLogger;
}

RVLWaveSettings* getWaveSettings() {
  return waveSettings;
}

}  // namespace State
