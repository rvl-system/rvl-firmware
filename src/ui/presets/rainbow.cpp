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
#include <vector>
#include "./settings.h"
#include "./ui/presets/rainbow.h"

namespace Rainbow {

uint8_t rate;

void updateWaveParameters() {
  RVLWaveSettings newSettings;
  newSettings.waves[0].h.a = 255;
  newSettings.waves[0].h.w_t = rate;
  newSettings.waves[0].h.w_x = 2;
  newSettings.waves[0].s.b = 255;
  newSettings.waves[0].v.b = 255;
  newSettings.waves[0].a.b = 255;
  rvl::setWaveSettings(&newSettings);
}

void updateRateValue(uint8_t newValue) {
  if (rate != newValue) {
    rate = newValue;
    Settings::setSetting("ui-r-rate", rate);
    updateWaveParameters();
  }
}

Rainbow::Rainbow() {
  rate = Settings::getSetting("ui-r-rate", 4);
  this->controls.push_back(new Control::RangeControl(
    "RATE",
    0,
    32,
    rate,
    updateRateValue,
    NULL));
}

void Rainbow::updateWave() {
  updateWaveParameters();
}

}  // namespace Rainbow
