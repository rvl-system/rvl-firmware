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

#include "./ui/presets/shift.hpp"
#include "./settings.hpp"
#include <Arduino.h>
#include <rvl.hpp>
#include <vector>

namespace Shift {

uint8_t hue;
uint8_t saturation;
uint8_t value;

void updateParametricSettings() {
  RVLParametricSettings newSettings;

  // b = 0.6, a = 0.3 so we go from 0.6 to 0.9, so blue to red
  newSettings.layers[0].h.a = 50;
  newSettings.layers[0].h.w_x = 2;
  newSettings.layers[0].h.b = 165;
  newSettings.layers[0].s.b = 255;
  newSettings.layers[0].v.b = 255;
  newSettings.layers[0].a.a = 255;
  newSettings.layers[0].a.w_t = 8;

  newSettings.layers[1].h.a = 75;
  newSettings.layers[1].h.w_x = 2;
  newSettings.layers[1].h.b = 165;
  newSettings.layers[1].h.phi = 127;
  newSettings.layers[1].s.b = 255;
  newSettings.layers[1].v.b = 255;
  newSettings.layers[1].a.b = 255;

  rvl::setParametricSettings(&newSettings);
}

// void updateHueValue(uint8_t newValue) {
//   if (hue != newValue) {
//     hue = newValue;
//     Settings::setSetting("ui-s-hue", hue);
//     updateParametricSettings();
//   }
// }

// void updateSaturationValue(uint8_t newValue) {
//   if (saturation != newValue) {
//     saturation = newValue;
//     Settings::setSetting("ui-s-sat", saturation);
//     updateParametricSettings();
//   }
// }

// void updateValueValue(uint8_t newValue) {
//   if (value != newValue) {
//     value = newValue;
//     Settings::setSetting("ui-sh-val", value);
//     updateParametricSettings();
//   }
// }

Shift::Shift() {
  // hue = Settings::getSetting("ui-sh-hue", 170);
  // saturation = Settings::getSetting("ui-s-sat", 255);
  // value = Settings::getSetting("ui-s-val", 255);
  // this->controls.push_back(
  //     new Control::RangeControl("Hue", 0, 255, hue, updateHueValue, NULL));
  // this->controls.push_back(new Control::RangeControl(
  //     "Saturation", 0, 255, saturation, updateSaturationValue, NULL));
  // this->controls.push_back(new Control::RangeControl(
  //     "Value", 0, 255, value, updateValueValue, NULL));
}

void Shift::updateParameters() {
  updateParametricSettings();
}

} // namespace Shift

#endif // HAS_UI
