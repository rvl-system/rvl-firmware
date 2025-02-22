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
#include <rvl-wifi.hpp>
#include <vector>

namespace Shift {

uint8_t hue;
uint8_t saturation;
uint8_t value;

void updateWaveParameters() {
  RVLWaveSettings newSettings;

  // Wave wave
  // b = 0.6, a = 0.3 so we go from 0.6 to 0.9, so blue to red
  newSettings.waves[0].h.a = 50;
  newSettings.waves[0].h.w_x = 2;
  newSettings.waves[0].h.b = 165;
  newSettings.waves[0].s.b = 255;
  newSettings.waves[0].v.b = 255;
  newSettings.waves[0].a.a = 255;
  newSettings.waves[0].a.w_t = 8;

  newSettings.waves[1].h.a = 75;
  newSettings.waves[1].h.w_x = 2;
  newSettings.waves[1].h.b = 165;
  newSettings.waves[1].h.phi = 127;
  newSettings.waves[1].s.b = 255;
  newSettings.waves[1].v.b = 255;
  newSettings.waves[1].a.b = 255;

  rvl::setWaveSettings(&newSettings);
}

// void updateHueValue(uint8_t newValue) {
//   if (hue != newValue) {
//     hue = newValue;
//     Settings::setSetting("ui-s-hue", hue);
//     updateWaveParameters();
//   }
// }

// void updateSaturationValue(uint8_t newValue) {
//   if (saturation != newValue) {
//     saturation = newValue;
//     Settings::setSetting("ui-s-sat", saturation);
//     updateWaveParameters();
//   }
// }

// void updateValueValue(uint8_t newValue) {
//   if (value != newValue) {
//     value = newValue;
//     Settings::setSetting("ui-sh-val", value);
//     updateWaveParameters();
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

void Shift::updateWave() {
  updateWaveParameters();
}

} // namespace Shift

#endif // HAS_UI
