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
#include <RVL-ESP.h>
#include <vector>
#include "./ui/presets/solid.h"

namespace Solid {

uint8_t hue = 170;
uint8_t saturation = 255;
uint8_t value = 255;

void updateWaveParameters() {
  RVLWaveSettings newSettings;

  // Wave wave
  newSettings.waves[0].h.b = hue;
  newSettings.waves[0].s.b = saturation;
  newSettings.waves[0].v.b = value;
  newSettings.waves[0].a.b = 255;

  RVLESPSetWaveSettings(&newSettings);
}

void updateHueValue(uint8_t newValue) {
  hue = newValue;
  updateWaveParameters();
}

void updateSaturationValue(uint8_t newValue) {
  saturation = newValue;
  updateWaveParameters();
}

void updateValueValue(uint8_t newValue) {
  value = newValue;
  updateWaveParameters();
}

Solid::Solid() {
  this->controls.push_back(new Control::RangeControl(
    "HUE",
    0,
    255,
    hue,
    updateHueValue));
  this->controls.push_back(new Control::RangeControl(
    "SAT",
    0,
    255,
    saturation,
    updateSaturationValue));
  this->controls.push_back(new Control::RangeControl(
    "VAL",
    0,
    255,
    value,
    updateValueValue));
}

void Solid::updateWave() {
  updateWaveParameters();
}

Solid solid;

}  // namespace Solid
