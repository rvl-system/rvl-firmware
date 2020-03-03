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

#define SUNRISE_HOUR 6
#define SUNRISE_MINUTE 0

#define SUNSET_HOUR 20
#define SUNSET_MINUTE 0

uint8_t hour;
uint8_t minute;
uint8_t brightness;

bool wifiConnected = false;
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

void onSynchronizationStateChanged(bool synchronized) {
  if (synchronized) {
    rvl::debug("System is synchronized");
  } else {
    rvl::debug("System is not synchronized");
  }
  Event::emit(Codes::EventType::SynchronizationChange);
}

void init() {
#ifdef DEFAULT_MODE_CONTROLLER
  RVLSetMode(RVLDeviceMode::Controller);
#else
  RVLSetMode(RVLDeviceMode::Receiver);
#endif
  RVLSetChannel(DEFAULT_CHANNEL);
  brightness = (DEFAULT_BRIGHTNESS * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / 16) + MIN_BRIGHTNESS;
#ifdef REMOTE_BRIGHTNESS
  RVLSetBrightness(brightness);
#endif
  RVLSetPowerState(true);
  RVLOnWaveSettingsUpdate(onWaveSettingsUpdated);
  RVLOnConnectionStateChanged(onConnectionStateChanged);
  RVLOnModeChanged(onModeChanged);
  RVLOnPowerStateChanged(onPowerStateChanged);
  RVLOnSynchronizationStateChage(onSynchronizationStateChanged);
#ifdef REMOTE_BRIGHTNESS
  RVLOnBrightnessChanged(onBrightnessChanged);
#endif
  rvl::info("State initialized");
}

void loop() {
}

bool getPowerState() {
  return RVLGetPowerState();
}

void setPowerState(bool powerState) {
  RVLSetPowerState(powerState);
}

bool isSynchronized() {
  return RVLGetSynchronizationState();
}

uint8_t getBrightness() {
#ifdef REMOTE_BRIGHTNESS
  brightness =  RVLGetBrightness();
  if (brightness > MAX_BRIGHTNESS) {
    brightness = MAX_BRIGHTNESS;
  }
  if (brightness < MIN_BRIGHTNESS) {
    brightness = MIN_BRIGHTNESS;
  }
#endif
  return brightness;
}

void setBrightness(uint8_t newBrightness) {
  brightness = newBrightness;
#ifdef REMOTE_BRIGHTNESS
  RVLSetBrightness(newBrightness);
#endif
}

bool isWifiConnected() {
  return wifiConnected;
}

void setWifiConnectedState(bool connected) {
  wifiConnected = connected;
  Event::emit(Codes::EventType::ConnectedStateChange);
}

RVLWaveSettings* getWaveSettings() {
  return waveSettings;
}

}  // namespace State
