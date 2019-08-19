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

#define SUNRISE_HOUR 10
#define SUNRISE_MINUTE 0

#define SUNSET_HOUR 20
#define SUNSET_MINUTE 0

uint8_t hour;
uint8_t minute;
uint8_t brightness;

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
#ifdef DEFAULT_MODE_CONTROLLER
  RVLESPSetMode(RVLDeviceMode::Controller);
#else
  RVLESPSetMode(RVLDeviceMode::Receiver);
#endif
  RVLESPSetChannel(DEFAULT_CHANNEL);
  brightness = (DEFAULT_BRIGHTNESS * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / 16) + MIN_BRIGHTNESS;
#ifdef REMOTE_BRIGHTNESS
  RVLESPSetBrightness(brightness);
#endif
  RVLESPSetPowerState(true);
  RVLESPOnWaveSettingsUpdate(onWaveSettingsUpdated);
  RVLESPOnConnectionStateChanged(onConnectionStateChanged);
  RVLESPOnModeChanged(onModeChanged);
  RVLESPOnPowerStateChanged(onPowerStateChanged);
#ifdef REMOTE_BRIGHTNESS
  RVLESPOnBrightnessChanged(onBrightnessChanged);
#endif
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
#ifdef REMOTE_BRIGHTNESS
  brightness =  RVLESPGetBrightness();
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
  RVLESPSetBrightness(newBrightness);
#endif
}

bool isWifiConnected() {
  return wifiConnected;
}

void setWifiConnectedState(bool connected) {
  wifiConnected = connected;
  Event::emit(Codes::EventType::ConnectedStateChange);
}

uint8_t getHour() {
  return hour;
}

uint8_t getMinute() {
  return minute;
}

void setTime(uint8_t newHour, uint8_t newMinute) {
  hour = newHour;
  minute = newMinute;
  Event::emit(Codes::EventType::TimeChange);
}

bool getTimerState() {
  bool timerState = false;
  if (hour < SUNRISE_HOUR) {
    timerState = true;
  } else if (hour == SUNRISE_HOUR) {
    if (minute < SUNRISE_MINUTE) {
      timerState = true;
    }
  } else if (hour == SUNSET_HOUR) {
    if (minute >= SUNSET_MINUTE) {
      timerState = true;
    }
  } else if (hour > SUNSET_HOUR) {
    timerState = true;
  }

  return timerState;
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
