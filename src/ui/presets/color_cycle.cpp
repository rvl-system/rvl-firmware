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

#ifdef HAS_UI

#include "./ui/presets/color_cycle.hpp"
#include "./settings.hpp"
#include <Arduino.h>
#include <rvl-wifi.h>
#include <vector>

namespace ColorCycle {

uint8_t rate;

void updateWaveParameters() {
  RVLWaveSettings newSettings;
  newSettings.waves[0].h.a = 255;
  newSettings.waves[0].h.w_t = rate;
  newSettings.waves[0].h.w_x = 0;
  newSettings.waves[0].s.b = 255;
  newSettings.waves[0].v.b = 255;
  newSettings.waves[0].a.b = 255;
  rvl::setWaveSettings(&newSettings);
}

void updateRateValue(uint8_t newValue) {
  if (rate != newValue) {
    rate = newValue;
    Settings::setSetting("ui-cc-rate", rate);
    updateWaveParameters();
  }
}

ColorCycle::ColorCycle() {
  rate = Settings::getSetting("ui-cc-rate", 4);
  this->controls.push_back(
      new Control::RangeControl("Rate", 0, 32, rate, updateRateValue, NULL));
}

void ColorCycle::updateWave() { updateWaveParameters(); }

} // namespace ColorCycle

#endif // HAS_UI
