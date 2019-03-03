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
// #include "./arduino_platform.h"
#include "./codes.h"
#include "./event.h"
#include "./logging.h"

namespace State {

#define MAX_LISTENERS 25

bool isActive = false;
uint32 idleStartTime = millis();

Settings settings;

Settings* getSettings() {
  return &settings;
}

void setClientId(uint16 id) {
  settings.id = id;
}

void setWaveParameters(WaveSettings *waveSettings) {
  memcpy(&settings.waveSettings, waveSettings, sizeof(WaveSettings));
  Event::emit(Codes::EventType::AnimationChange);
}

void init() {
  Logging::info("State initialized");
}

void setMode(Codes::Mode::Mode newMode) {
  if (settings.mode == newMode) {
    return;
  }
  switch (newMode) {
    case Codes::Mode::Controller:
      Logging::info("Changing to controller mode");
      settings.mode = newMode;
      Event::emit(Codes::EventType::ModeChange);
      break;
    case Codes::Mode::Receiver:
      Logging::info("Changing to receiver mode");
      settings.mode = newMode;
      Event::emit(Codes::EventType::ModeChange);
      break;
    default:
      Logging::error("Tried to set unknown mode %d", newMode);
  }
}

int32 clockOffset = 0;
void setClockOffset(int32 newOffset) {
  clockOffset = newOffset;
  Logging::debug("Setting clock offset: %d, clock: ", clockOffset, millis() + clockOffset);
}

uint32 clock = millis();
bool wifiConnected = false;
uint8 brightness = 0;

void loop() {
  // clock = millis() + ArduinoPlatform::platform.getClockOffset();
  settings.clock = millis() + clockOffset;
}

uint32 getAnimationClock() {
  return clock;
}

uint8 getBrightness() {
  return brightness;
}

void setBrightness(uint8 newBrightness) {
  brightness = newBrightness;
}

bool isWifiConnected() {
  return wifiConnected;
}

void setWifiConnectedState(bool connected) {
  wifiConnected = connected;
  Event::emit(Codes::EventType::ConnectedStateChange);
}

}  // namespace State
