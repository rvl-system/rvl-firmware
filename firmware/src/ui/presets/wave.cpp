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
#include <rvl-esp.h>
#include <vector>
#include "./ui/presets/wave.h"

namespace Wave {

uint8_t rate = 8;
uint8_t waveHue = 0;
uint8_t foregroundHue = 170;
uint8_t backgroundHue = 85;

void updateWaveParameters() {
  RVLWaveSettings newSettings;

  // Wave wave
  newSettings.waves[0].h.b = waveHue;
  newSettings.waves[0].s.b = 255;
  newSettings.waves[0].v.b = 255;
  newSettings.waves[0].a.a = 255;
  newSettings.waves[0].a.w_t = rate;
  newSettings.waves[0].a.w_x = 2;

  // Foreground wave
  newSettings.waves[1].h.b = foregroundHue;
  newSettings.waves[1].s.b = 255;
  newSettings.waves[1].v.b = 255;
  newSettings.waves[1].a.w_t = rate;
  newSettings.waves[1].a.a = 255;

  // Background wave
  newSettings.waves[2].h.b = backgroundHue;
  newSettings.waves[2].s.b = 255;
  newSettings.waves[2].v.b = 255;
  newSettings.waves[2].a.a = 255;

  RVLESPSetWaveSettings(&newSettings);
}

void updateRateValue(uint8_t newValue) {
  rate = newValue;
  updateWaveParameters();
}

void updateWaveHueValue(uint8_t newValue) {
  waveHue = newValue;
  updateWaveParameters();
}

void updateForegroundHueValue(uint8_t newValue) {
  foregroundHue = newValue;
  updateWaveParameters();
}

void updateBackgroundHueValue(uint8_t newValue) {
  backgroundHue = newValue;
  updateWaveParameters();
}

Wave::Wave() {
  this->controls.push_back(new Control::RangeControl(
    "RATE",
    0,
    32,
    rate,
    updateRateValue));
  this->controls.push_back(new Control::RangeControl(
    "WHUE",
    0,
    255,
    waveHue,
    updateWaveHueValue));
  this->controls.push_back(new Control::RangeControl(
    "FHUE",
    0,
    255,
    foregroundHue,
    updateForegroundHueValue));
  this->controls.push_back(new Control::RangeControl(
    "BHUE",
    0,
    255,
    backgroundHue,
    updateBackgroundHueValue));
}

void Wave::updateWave() {
  updateWaveParameters();
}

Wave wave;

}  // namespace Wave
