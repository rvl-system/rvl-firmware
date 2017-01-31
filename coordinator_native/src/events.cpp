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
#include "codes.h"
#include "screen.h"

void emitControlEvent(Codes::Control::Control currentControl) {
  Serial.print("Setting control ");
  Serial.println(currentControl);

  Screen::updateControl(currentControl);
}

void emitValueEvent(Codes::Preset::Preset preset, int code, int newValue) {
  Serial.print("Setting preset ");
  Serial.print(preset);
  Serial.print(" code ");
  Serial.print(code);
  Serial.print(" to value ");
  Serial.println(newValue);

  Screen::updateValue(preset, code, newValue);
}

void emitBrightnessEvent(int brightness) {
  Serial.print("Setting brightness to ");
  Serial.println(brightness);

  Screen::updateBrightness(brightness);
}

void emitPresetEvent(Codes::Preset::Preset preset) {
  Serial.print("Setting preset ");
  Serial.println(preset);

  Screen::updatePreset(preset);
}

void emitClientEvent(int numClients) {
  Serial.print("Setting num clients to ");
  Serial.println(numClients);

  Screen::updateClientCount(numClients);
}

void emitIdleEvent(Codes::IdleState::IdleState idleState) {
  Serial.print("Setting idle state to ");
  Serial.println(idleState);

  Screen::updateIdleState(idleState);
}
