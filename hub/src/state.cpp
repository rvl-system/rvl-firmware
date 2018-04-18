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
#include "state.h"
#include "codes.h"
#include "events.h"

namespace State {

  #define BRIGHTNESS_STEP 1
  #define MAX_BRIGHTNESS 255
  #define DIM_TIMEOUT 10000
  #define OFF_TIMEOUT 5000

  bool isActive = false;
  unsigned long idleStartTime = millis();

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

    Events::emitControlEvent(settings.currentControl);
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
    Events::emitValueEvent(settings.preset, code, newValue);
  }

  void controlUp() {
    switch(settings.currentControl) {
      case Codes::Control::Brightness:
        if (settings.brightness < MAX_BRIGHTNESS) {
          settings.brightness += BRIGHTNESS_STEP;
          if (settings.brightness > MAX_BRIGHTNESS) {
            settings.brightness = MAX_BRIGHTNESS;
          }
          Events::emitBrightnessEvent(settings.brightness);
        }
        break;
      case Codes::Control::Preset:
        settings.preset++;
        if (settings.preset == NUM_PRESETS) {
          settings.preset = 0;
        }
        Events::emitPresetEvent(settings.preset);
        break;
      default:
        handleValueChange(settings.currentControl - 3, true);
        break;
    }
  }

  void controlDown() {
    switch(settings.currentControl) {
      case Codes::Control::Brightness:
        if (settings.brightness > 0) {
          settings.brightness -= BRIGHTNESS_STEP;
          if (settings.brightness < 0) {
            settings.brightness = 0;
          }
          Events::emitBrightnessEvent(settings.brightness);
        }
        break;
      case Codes::Control::Preset:
        switch(settings.preset) {
          case Codes::Preset::Fade:
            settings.preset = Codes::Preset::Pulse;
            break;
          case Codes::Preset::Pulse:
            settings.preset = Codes::Preset::Fade;
            break;
        }
        Events::emitPresetEvent(settings.preset);
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
    Events::emitClientEvent(settings.numClients);
  }

  void setActive() {
    isActive = true;
    settings.idleState = Codes::IdleState::Active;
    Events::emitIdleEvent(settings.idleState);
  }

  void setIdling() {
    idleStartTime = millis();
    isActive = false;
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

  void loop() {
    if (!isActive) {
      switch(settings.idleState) {
        case Codes::IdleState::Active:
          if (millis() >= idleStartTime + DIM_TIMEOUT) {
            idleStartTime = millis();
            settings.idleState = Codes::IdleState::ShallowIdle;
            Events::emitIdleEvent(settings.idleState);
          }
          break;
        case Codes::IdleState::ShallowIdle:
          if (millis() >= idleStartTime + OFF_TIMEOUT) {
            settings.idleState = Codes::IdleState::DeepIdle;
            Events::emitIdleEvent(settings.idleState);
          }
          break;
      }
    }
  }

}
