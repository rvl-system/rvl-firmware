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

void onPowerStateChanged(bool powerState) {
  Event::emit(Codes::EventType::PowerStateChange);
}

void onBrightnessChanged(uint8_t brightness) {
  Event::emit(Codes::EventType::BrightnessChange);
}

void init() {
  RVLESPSetMode(RVLDeviceMode::Receiver);
  RVLESPSetChannel(DEFAULT_CHANNEL);
  RVLESPSetBrightness(DEFAULT_BRIGHTNESS);
  RVLESPSetPowerState(true);
  RVLESPOnWaveSettingsUpdate(onWaveSettingsUpdated);
  RVLESPOnConnectionStateChanged(onConnectionStateChanged);
  RVLESPOnModeChanged(onModeChanged);
  RVLESPOnPowerStateChanged(onPowerStateChanged);
  RVLESPOnBrightnessChanged(onBrightnessChanged);
  State::getLogger()->info("State initialized");
}

void loop() {
}

bool getPowerState() {
  return RVLESPGetPowerState();
}

void setPowerState(bool powerState) {
  RVLESPSetPowerState(powerState);
}

uint8_t getBrightness() {
  uint8_t brightness =  RVLESPGetBrightness();
  if (brightness > MAX_BRIGHTNESS) {
    brightness = MAX_BRIGHTNESS;
  }
  if (brightness < MIN_BRIGHTNESS) {
    brightness = MIN_BRIGHTNESS;
  }
  return brightness;
}

void setBrightness(uint8_t newBrightness) {
  RVLESPSetBrightness(newBrightness);
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
