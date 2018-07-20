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
#include "./presets/preset_control_set.h"
#include "./presets/rainbow.h"
#include "./presets/pulse.h"
#include "./presets/wave.h"
#include "./presets/color_cycle.h"

namespace UIState {

uint8 currentControl = 0;
uint8 preset = DEFAULT_PRESET;

PresetControlSet* presets[] = {
  &Rainbow::rainbow,
  &Pulse::pulse,
  &Wave::wave,
  &ColorCycle::colorCycle
};

void updateBrightnessValue(uint8 newValue) {
  State::getSettings()->brightness = newValue;
  Event::emit(Codes::EventType::AnimationChange);
}
Control::RangeControl brightnessControl(
  "BRT",
  0,
  MAX_BRIGHTNESS,
  DEFAULT_BRIGHTNESS,
  updateBrightnessValue);

void updateModeValue(uint8 selectedValueIndex) {
  if (State::getSettings()->mode != selectedValueIndex) {
    switch (selectedValueIndex) {
      case Codes::Mode::Controller:
        State::setMode(Codes::Mode::Controller);
        presets[preset]->updateWave();
        break;
      case Codes::Mode::Receiver:
        State::setMode(Codes::Mode::Receiver);
        break;
    }
  }
}
Control::ListControl modeControl(
  "MODE",
  { "Controller", "Receiver" },
  1,
  updateModeValue);

void updatePresetValue(uint8 selectedValueIndex) {
  if (UIState::preset != selectedValueIndex) {
    UIState::preset = selectedValueIndex;
    presets[preset]->updateWave();
  }
}
Control::ListControl presetControl(
  "PRST",
  { "Rainbow", "Pulse", "Wave", "Color Cycle" },
  0,
  updatePresetValue);

std::vector<Control::Control*> controls = {
  &brightnessControl,
  &modeControl
};

void update() {
  auto settings = State::getSettings();
  while (controls.size() > 2) {
    controls.pop_back();
  }
  if (settings->mode == Codes::Mode::Controller) {
    controls.push_back(&presetControl);
    for (auto& control : presets[preset]->controls) {
      controls.push_back(control);
    }
  }
  Event::emit(Codes::EventType::UIStateChange);
}

void init() {
  Event::on(Codes::EventType::ConnectedStateChange, update);
  Event::on(Codes::EventType::AnimationChange, update);
  Event::on(Codes::EventType::ModeChange, update);
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
