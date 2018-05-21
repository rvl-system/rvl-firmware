/*
Copyright (c) 2018 Bryan Hughes <bryan@nebri.us>

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
#include "./ui/ui_state.h"
#include "./state.h"
#include "./event.h"
#include "./codes.h"

namespace UIState {

#define BRIGHTNESS_STEP 1
#define MAX_BRIGHTNESS 255

uint8 currentControl = 0;

void update() {
  Event::emit(Codes::EventType::UIStateChange);
}

void init() {
  Event::on(Codes::EventType::ConnectedStateChange, update);
}

void nextControl() {
  int maxControls = numControls[State::getSettings()->presetSettings.preset];
  if (currentControl < maxControls - 1) {
    currentControl++;
    Serial.print("Setting control ");
    Serial.println(currentControl);
    Event::emit(Codes::EventType::UIStateChange);
  }
}

void previousControl() {
  if (currentControl > 0) {
    currentControl--;
    Serial.print("Setting control ");
    Serial.println(currentControl);
    Event::emit(Codes::EventType::UIStateChange);
  }
}

int calculateNewValue(byte code, int value, bool direction) {
  auto settings = State::getSettings();
  if (direction) {
    value++;
    if (value > presetValueMax[settings->presetSettings.preset][code]) {
      value = presetValueMax[settings->presetSettings.preset][code];
    }
  } else {
    value--;
    if (value < presetValueMin[settings->presetSettings.preset][code]) {
      value = presetValueMin[settings->presetSettings.preset][code];
    }
  }
  return value;
}

void handleValueChange(int code, bool direction) {
  auto settings = State::getSettings();
  int newValue = calculateNewValue(
    code,
    settings->presetSettings.presetValues[settings->presetSettings.preset][code],
    direction);
  settings->presetSettings.presetValues[settings->presetSettings.preset][code] = newValue;

  Serial.print("Setting preset ");
  Serial.print(settings->presetSettings.preset);
  Serial.print(" code ");
  Serial.print(code);
  Serial.print(" to value ");
  Serial.println(newValue);
  Event::emit(Codes::EventType::AnimationChange);
}

void controlIncrease() {
  auto settings = State::getSettings();
  switch (currentControl) {
    case 0:
      if (settings->brightness < MAX_BRIGHTNESS) {
        settings->brightness += BRIGHTNESS_STEP;
        if (settings->brightness > MAX_BRIGHTNESS) {
          settings->brightness = MAX_BRIGHTNESS;
        }
        Serial.print("Setting brightness to ");
        Serial.println(settings->brightness);
        Event::emit(Codes::EventType::AnimationChange);
      }
      break;
      // TODO(nebrius): handle values 1 and 2
    case 3:
      settings->presetSettings.preset++;
      if (settings->presetSettings.preset == NUM_PRESETS) {
        settings->presetSettings.preset = 0;
      }
      Serial.print("Setting preset ");
      Serial.println(settings->presetSettings.preset);
      Event::emit(Codes::EventType::AnimationChange);
      break;
    default:
      handleValueChange(currentControl - 4, true);
      break;
  }
  Event::emit(Codes::EventType::UIStateChange);
}

void controlDecrease() {
  auto settings = State::getSettings();
  switch (currentControl) {
    case 0:
      if (settings->brightness > 0) {
        settings->brightness -= BRIGHTNESS_STEP;
        if (settings->brightness < 0) {
          settings->brightness = 0;
        }
        Serial.print("Setting brightness to ");
        Serial.println(settings->brightness);
        Event::emit(Codes::EventType::AnimationChange);
      }
      break;
      // TODO(nebrius): handle values 1 and 2
    case 3:
      settings->presetSettings.preset--;
      if (settings->presetSettings.preset < 0) {
        settings->presetSettings.preset = NUM_PRESETS - 1;
      }
      Serial.print("Setting preset ");
      Serial.println(settings->presetSettings.preset);
      Event::emit(Codes::EventType::AnimationChange);
      break;
    default:
      handleValueChange(currentControl - 4, false);
      break;
  }
  Event::emit(Codes::EventType::UIStateChange);
}

bool isCurrentControlRange() {
  return true;
}

}  // namespace UIState
