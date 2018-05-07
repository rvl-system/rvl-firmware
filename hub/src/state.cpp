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

#define BRIGHTNESS_STEP 1
#define MAX_BRIGHTNESS 255
#define DIM_TIMEOUT 10000
#define OFF_TIMEOUT 5000
#define MAX_LISTENERS 255

bool isActive = false;
uint32 idleStartTime = millis();

Settings settings;

Settings* getSettings() {
  return &settings;
}

void nextControl() {
  int maxControls = 3;
  for (int i = 0; i < NUM_PRESET_VALUES; i++) {
    if (presetValueLabels[settings.preset][i] == NULL) {
      break;
    }
    maxControls++;
  }
  settings.currentControl++;
  if (settings.currentControl == maxControls) {
    settings.currentControl = 0;
  }

  Serial.print("Setting control ");
  Serial.println(settings.currentControl);
  Event::emit(Codes::EventType::InputChange);
}

int calculateNewValue(byte code, int value, bool direction) {
  if (direction) {
    value++;
    if (value > presetValueMax[settings.preset][code]) {
      value = presetValueMax[settings.preset][code];
    }
  } else {
    value--;
    if (value < presetValueMin[settings.preset][code]) {
      value = presetValueMin[settings.preset][code];
    }
  }
  return value;
}

void handleValueChange(int code, bool direction) {
  int newValue = calculateNewValue(code, settings.presetValues[settings.preset][code], direction);
  settings.presetValues[settings.preset][code] = newValue;

  Serial.print("Setting preset ");
  Serial.print(settings.preset);
  Serial.print(" code ");
  Serial.print(code);
  Serial.print(" to value ");
  Serial.println(newValue);
  Event::emit(Codes::EventType::AnimationChange);
}

void controlUp() {
  switch (settings.currentControl) {
    case Codes::Control::Brightness:
      if (settings.brightness < MAX_BRIGHTNESS) {
        settings.brightness += BRIGHTNESS_STEP;
        if (settings.brightness > MAX_BRIGHTNESS) {
          settings.brightness = MAX_BRIGHTNESS;
        }
        Serial.print("Setting brightness to ");
        Serial.println(settings.brightness);
        Event::emit(Codes::EventType::AnimationChange);
      }
      break;
    case Codes::Control::Preset:
      settings.preset++;
      if (settings.preset == NUM_PRESETS) {
        settings.preset = 0;
      }
      Serial.print("Setting preset ");
      Serial.println(settings.preset);
      Event::emit(Codes::EventType::AnimationChange);
      break;
    default:
      handleValueChange(settings.currentControl - 3, true);
      break;
  }
}

void controlDown() {
  switch (settings.currentControl) {
    case Codes::Control::Brightness:
      if (settings.brightness > 0) {
        settings.brightness -= BRIGHTNESS_STEP;
        if (settings.brightness < 0) {
          settings.brightness = 0;
        }
        Serial.print("Setting brightness to ");
        Serial.println(settings.brightness);
        Event::emit(Codes::EventType::AnimationChange);
      }
      break;
    case Codes::Control::Preset:
      switch (settings.preset) {
        case Codes::Preset::Fade:
          settings.preset = Codes::Preset::Pulse;
          break;
        case Codes::Preset::Pulse:
          settings.preset = Codes::Preset::Fade;
          break;
      }
      Serial.print("Setting preset ");
      Serial.println(settings.preset);
      Event::emit(Codes::EventType::AnimationChange);
      break;
    default:
      handleValueChange(settings.currentControl - 3, false);
      break;
  }
}

void setClientsConnected(int numClients) {
  if (numClients == settings.numClients) {
    return;
  }
  settings.numClients = numClients;

  Serial.print("Setting num clients to ");
  Serial.println(settings.numClients);
  Event::emit(Codes::EventType::InputChange);
}

void setAnimation(uint32 commandTime, uint8 preset, uint8* presetValues) {
  settings.commandTime = commandTime;
  settings.preset = preset;
  for (int i = 0; i < NUM_PRESET_VALUES; i++) {
    settings.presetValues[preset][i] = presetValues[i];
  }
  Event::emit(Codes::EventType::AnimationChange);
}

void init() {
  settings.presetValues = new byte*[NUM_PRESETS];
  for (int i = 0; i < NUM_PRESETS; i++) {
    settings.presetValues[i] = new byte[NUM_PRESET_VALUES];
    for (int j = 0; j < NUM_PRESET_VALUES; j++) {
      settings.presetValues[i][j] = presetValueDefaults[i][j];
    }
  }
  Serial.println("State initialized");
}

}  // namespace State
