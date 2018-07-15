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
#include <vector>
#include "./ui/controls/control.h"
#include "./ui/controls/base_controls.h"
#include "./state.h"
#include "./event.h"

namespace BaseControls {

void increaseBrightnessValue() {
  auto settings = State::getSettings();
  if (settings->brightness < 255) {
    settings->brightness++;
    Event::emit(Codes::EventType::AnimationChange);
  }
}
void decreaseBrightnessValue() {
  auto settings = State::getSettings();
  if (settings->brightness > 0) {
    settings->brightness--;
    Event::emit(Codes::EventType::AnimationChange);
  }
}
uint8 getBrightnessValue() {
  return State::getSettings()->brightness;
}
Control::RangeControl brightnessControl(
  increaseBrightnessValue,
  decreaseBrightnessValue,
  getBrightnessValue,
  "BRT");

void increaseWifiValue() {
  // TODO(nebrius): implement me
}
void decreaseWifiValue() {
  // TODO(nebrius): implement me
}
uint8 getWifiValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::ListControl wifiControl(
  increaseWifiValue,
  decreaseWifiValue,
  getWifiValue,
  "WIFI",
  { "RVL1", "RVL2", "RVL3", "RVL4" });

void increaseModeValue() {
  auto settings = State::getSettings();
  if (settings->mode == Codes::Mode::Controller) {
    State::setMode(Codes::Mode::Receiver);
  } else {
    State::setMode(Codes::Mode::Controller);
  }
}
void decreaseModeValue() {
  auto settings = State::getSettings();
  if (settings->mode == Codes::Mode::Controller) {
    State::setMode(Codes::Mode::Receiver);
  } else {
    State::setMode(Codes::Mode::Controller);
  }
}
uint8 getModeValue() {
  return State::getSettings()->mode;
}
Control::ListControl modeControl(
  increaseModeValue,
  decreaseModeValue,
  getModeValue,
  "MODE",
  { "Controller", "Receiver" });

void increasePresetValue() {
  auto settings = State::getSettings();
  settings->presetSettings.preset++;
  if (settings->presetSettings.preset == NUM_PRESETS) {
    settings->presetSettings.preset = 0;
  }
  Event::emit(Codes::EventType::AnimationChange);
}
void decreasePresetValue() {
  auto settings = State::getSettings();
  settings->presetSettings.preset--;
  if (settings->presetSettings.preset < 0) {
    settings->presetSettings.preset = NUM_PRESETS - 1;
  }
  Event::emit(Codes::EventType::AnimationChange);
}
uint8 getPresetValue() {
  return State::getSettings()->presetSettings.preset;
}
Control::ListControl presetControl(
  increasePresetValue,
  decreasePresetValue,
  getPresetValue,
  "PRST",
  { "Rainbow", "Pulse", "Wave" });

}  // namespace BaseControls
