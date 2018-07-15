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

void setWifiConnected(bool connected) {
  settings.wifiConnected = connected;
  Event::emit(Codes::EventType::ConnectedStateChange);
}

void setAnimation(uint8 preset, uint8* presetValues) {
  settings.presetSettings.preset = preset;
  for (int i = 0; i < NUM_PRESET_VALUES; i++) {
    settings.presetSettings.presetValues[preset][i] = presetValues[i];
  }
  Event::emit(Codes::EventType::AnimationChange);
}

void init() {
  settings.presetSettings.presetValues = new byte*[NUM_PRESETS];
  for (int i = 0; i < NUM_PRESETS; i++) {
    settings.presetSettings.presetValues[i] = new byte[NUM_PRESET_VALUES];
    for (int j = 0; j < NUM_PRESET_VALUES; j++) {
      settings.presetSettings.presetValues[i][j] = presetValueDefaults[i][j];
    }
  }
  Serial.println("State initialized");
}

void setMode(Codes::Mode::Mode newMode) {
  switch (newMode) {
    case Codes::Mode::Controller:
      Serial.println("Changing to controller mode");
      settings.mode = newMode;
      break;
    case Codes::Mode::Receiver:
      Serial.println("Changing to receiver mode");
      settings.mode = newMode;
      break;
    default:
      Serial.print("Error: tried to set unknown mode ");
      Serial.println(newMode);
  }
}

int32 clockOffset = 0;
void setClockOffset(int32 newOffset) {
  clockOffset = newOffset;
  Serial.print("Setting clock offset: ");
  Serial.print(clockOffset);
  Serial.print(", clock: ");
  Serial.println(millis() + clockOffset);
}

void loop() {
  settings.clock = millis() + clockOffset;
}

}  // namespace State
