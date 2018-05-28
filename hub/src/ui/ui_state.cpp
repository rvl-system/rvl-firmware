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
#include "./ui/controls/base_controls.h"
#include "./ui/controls/preset_controls.h"
#include "./state.h"
#include "./event.h"
#include "./codes.h"

#define NUM_BASE_CONTROLS 4

namespace UIState {

uint8 currentControl = 0;
uint8 currentPreset = 255;

std::vector<Control::Control*> controls = {
  &BaseControls::brightnessControl,
  &BaseControls::wifiControl,
  &BaseControls::modeControl,
  &BaseControls::presetControl
};

void update() {
  auto settings = State::getSettings();
  if (settings->presetSettings.preset != currentPreset) {
    currentPreset = settings->presetSettings.preset;
    while (controls.size() > NUM_BASE_CONTROLS) {
      controls.pop_back();
    }
    switch (settings->presetSettings.preset) {
      case Codes::Preset::Rainbow:
        controls.push_back(&PresetControls::rainbowRateControl);
        break;
      case Codes::Preset::Pulse:
        controls.push_back(&PresetControls::pulseRateControl);
        controls.push_back(&PresetControls::pulseHueControl);
        controls.push_back(&PresetControls::pulseSaturationControl);
        break;
      case Codes::Preset::Wave:
        controls.push_back(&PresetControls::waveRateControl);
        controls.push_back(&PresetControls::waveForegroundHueControl);
        controls.push_back(&PresetControls::waveForegroundSaturationControl);
        controls.push_back(&PresetControls::waveBackgroundHueControl);
        controls.push_back(&PresetControls::waveBackgroundSaturationControl);
        break;
    }
  }
  Event::emit(Codes::EventType::UIStateChange);
}

void init() {
  Event::on(Codes::EventType::ConnectedStateChange, update);
  Event::on(Codes::EventType::AnimationChange, update);
  update();
  controls.reserve(10);
}

void nextControl() {
  if (currentControl < controls.size() - 1) {
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

void controlIncrease() {
  controls[currentControl]->increaseValue();
  Event::emit(Codes::EventType::UIStateChange);
}

void controlDecrease() {
  controls[currentControl]->decreaseValue();
  Event::emit(Codes::EventType::UIStateChange);
}

bool isCurrentControlRange() {
  return controls[currentControl]->type == Control::ControlType::Range;
}

}  // namespace UIState
