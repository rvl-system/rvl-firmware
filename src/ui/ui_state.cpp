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
#include <rvl-wifi.h>
#include "./ui/ui_state.h"
#include "./state.h"
#include "./codes.h"
#include "./presets/preset_control_set.h"
#include "./presets/rainbow.h"
#include "./presets/pulse.h"
#include "./presets/wave.h"
#include "./presets/solid.h"
#include "./presets/color_cycle.h"

namespace UIState {

uint8_t currentControl = 0;
uint8_t preset = DEFAULT_PRESET;

#define NUM_GLOBAL_CONTROLS 3

PresetControlSet* presets[] = {
  &Rainbow::rainbow,
  &Pulse::pulse,
  &Wave::wave,
  &ColorCycle::colorCycle,
  &Solid::solid
};

uint8_t getBrightnessValue() {
  return 16 * (State::getBrightness() - MIN_BRIGHTNESS) / (MAX_BRIGHTNESS - MIN_BRIGHTNESS);
}
void updateBrightnessValue(uint8_t newValue) {
  uint16_t adjustedBrightness = (newValue * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / 16) + MIN_BRIGHTNESS;
  State::setBrightness(adjustedBrightness);
  rvl::info("Changing brightness to %d", adjustedBrightness);
  rvl::emit(Codes::EventType::AnimationChange);
}
Control::RangeControl brightnessControl(
  "BRT",
  0,
  16,
  DEFAULT_BRIGHTNESS,
  updateBrightnessValue,
  getBrightnessValue);

void updateChannelValue(uint8_t selectedValueIndex) {
  if (RVLGetChannel() != selectedValueIndex) {
    RVLSetChannel(selectedValueIndex);
  }
}
Control::ListControl channelControl(
  "CHNL",
  { "0", "1", "2", "3", "4", "5", "6", "7" },
  DEFAULT_CHANNEL,
  updateChannelValue);

void updateModeValue(uint8_t selectedValueIndex) {
  if (RVLGetMode() != static_cast<RVLDeviceMode>(selectedValueIndex)) {
    switch (static_cast<RVLDeviceMode>(selectedValueIndex)) {
      case RVLDeviceMode::Controller:
        RVLSetMode(RVLDeviceMode::Controller);
        presets[preset]->updateWave();
        break;
      case RVLDeviceMode::Receiver:
        RVLSetMode(RVLDeviceMode::Receiver);
        break;
    }
  }
}
Control::ListControl modeControl(
  "MODE",
  { "Controller", "Receiver" },
#ifdef DEFAULT_MODE_CONTROLLER
  0,
#else
  1,
#endif
  updateModeValue);

void updatePresetValue(uint8_t selectedValueIndex) {
  if (UIState::preset != selectedValueIndex) {
    UIState::preset = selectedValueIndex;
    presets[preset]->updateWave();
  }
}
Control::ListControl presetControl(
  "PRST",
  { "Rainbow", "Pulse", "Wave", "Color Cycle", "Solid" },
  2,
  updatePresetValue);

std::vector<Control::Control*> controls = {
  &brightnessControl,
  &channelControl,
  &modeControl
};

void update() {
  while (controls.size() > NUM_GLOBAL_CONTROLS) {
    controls.pop_back();
  }
  if (RVLGetMode() == RVLDeviceMode::Controller) {
    controls.push_back(&presetControl);
    for (auto& control : presets[preset]->controls) {
      controls.push_back(control);
    }
  }
  rvl::emit(Codes::EventType::UIStateChange);
}

void init() {
  rvl::on(Codes::EventType::ConnectedStateChange, update);
  rvl::on(Codes::EventType::AnimationChange, update);
  rvl::on(Codes::EventType::ModeChange, update);
  rvl::on(Codes::EventType::BrightnessChange, update);
  rvl::on(Codes::EventType::TimeChange, update);
  update();
  controls.reserve(10);
  presets[preset]->updateWave();
}

void nextControl() {
  if (currentControl < controls.size() - 1) {
    currentControl++;
    rvl::debug("Setting control to %d", currentControl);
    rvl::emit(Codes::EventType::UIStateChange);
  }
}

void previousControl() {
  if (currentControl > 0) {
    currentControl--;
    rvl::debug("Setting control to %d", currentControl);
    rvl::emit(Codes::EventType::UIStateChange);
  }
}

void controlIncrease() {
  controls[currentControl]->increaseValue();
  rvl::emit(Codes::EventType::UIStateChange);
}

void controlDecrease() {
  controls[currentControl]->decreaseValue();
  rvl::emit(Codes::EventType::UIStateChange);
}

bool isCurrentControlRange() {
  return controls[currentControl]->type == Control::ControlType::Range;
}

}  // namespace UIState
