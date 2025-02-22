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

#include "./ui/presets/pulse.hpp"
#include "./settings.hpp"
#include <Arduino.h>
#include <rvl-wifi.hpp>
#include <vector>

namespace Pulse {

uint8_t rate;
uint8_t hue;
uint8_t saturation;

void updateWaveParameters() {
  RVLWaveSettings newSettings;
  newSettings.waves[0].h.b = hue;
  newSettings.waves[0].s.b = saturation;
  newSettings.waves[0].v.a = 255;
  newSettings.waves[0].v.w_t = rate;
  newSettings.waves[0].a.b = 255;
  rvl::setWaveSettings(&newSettings);
}

void updateRateValue(uint8_t newValue) {
  if (rate != newValue) {
    rate = newValue;
    Settings::setSetting("ui-p-rate", rate);
    updateWaveParameters();
  }
}

void updateHueValue(uint8_t newValue) {
  if (hue != newValue) {
    hue = newValue;
    Settings::setSetting("ui-p-hue", hue);
    updateWaveParameters();
  }
}

void updateSaturationValue(uint8_t newValue) {
  if (saturation != newValue) {
    saturation = newValue;
    Settings::setSetting("ui-p-sat", saturation);
    updateWaveParameters();
  }
}

Pulse::Pulse() {
  rate = Settings::getSetting("ui-p-rate", 16);
  hue = Settings::getSetting("ui-p-hue", 120);
  saturation = Settings::getSetting("ui-p-sat", 255);
  this->controls.push_back(
      new Control::RangeControl("Rate", 0, 32, rate, updateRateValue, NULL));
  this->controls.push_back(
      new Control::RangeControl("Hue", 0, 255, hue, updateHueValue, NULL));
  this->controls.push_back(new Control::RangeControl(
      "Saturation", 0, 255, saturation, updateSaturationValue, NULL));
}

void Pulse::updateWave() {
  updateWaveParameters();
}

} // namespace Pulse

#endif // HAS_UI
