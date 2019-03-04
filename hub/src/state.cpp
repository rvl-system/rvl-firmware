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
#include "./arduino_platform.h"
#include "./codes.h"
#include "./event.h"
#include "./logging.h"

namespace State {

uint8 brightness = DEFAULT_BRIGHTNESS;
bool wifiConnected = false;
uint32_t clock = millis();

void init() {
  ArduinoPlatform::platform.setDeviceMode(RVLDeviceMode::Controller);
  Logging::info("State initialized");
}

void loop() {
  clock = millis() + ArduinoPlatform::platform.getClockOffset();
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
