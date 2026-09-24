/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

This file is part of RVL Firmware.

RVL Firmware is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RVL Firmware is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RVL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAS_UI

#include "./ui/presets/solid.hpp"
#include "./settings.hpp"
#include <Arduino.h>
#include <rvl.hpp>
#include <vector>

namespace Solid {

uint8_t hue;
uint8_t saturation;
uint8_t value;

void updateParametricSettings() {
  RVLParametricSettings newSettings;

  newSettings.layers[0].h.b = hue;
  newSettings.layers[0].s.b = saturation;
  newSettings.layers[0].v.b = value;
  newSettings.layers[0].a.b = 255;

  rvl::setParametricSettings(&newSettings);
}

void updateHueValue(uint8_t newValue) {
  if (hue != newValue) {
    hue = newValue;
    Settings::setSetting("ui-s-hue", hue);
    updateParametricSettings();
  }
}

void updateSaturationValue(uint8_t newValue) {
  if (saturation != newValue) {
    saturation = newValue;
    Settings::setSetting("ui-s-sat", saturation);
    updateParametricSettings();
  }
}

void updateValueValue(uint8_t newValue) {
  if (value != newValue) {
    value = newValue;
    Settings::setSetting("ui-s-val", value);
    updateParametricSettings();
  }
}

Solid::Solid() {
  hue = Settings::getSetting("ui-s-hue", 170);
  saturation = Settings::getSetting("ui-s-sat", 255);
  value = Settings::getSetting("ui-s-val", 255);
  this->controls.push_back(
      new Control::RangeControl("Hue", 0, 255, hue, updateHueValue, NULL));
  this->controls.push_back(new Control::RangeControl(
      "Saturation", 0, 255, saturation, updateSaturationValue, NULL));
  this->controls.push_back(new Control::RangeControl(
      "Value", 0, 255, value, updateValueValue, NULL));
}

void Solid::updateParameters() {
  updateParametricSettings();
}

} // namespace Solid

#endif // HAS_UI
