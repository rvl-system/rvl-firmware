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

#include <Arduino.h>
#include <rvl-wifi.h>
#include <vector>
#include "./settings.h"
#include "./ui/presets/wave.h"

namespace Wave {

uint8_t rate;
uint8_t waveHue;
uint8_t foregroundHue;
uint8_t backgroundHue;

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

  rvl::setWaveSettings(&newSettings);
}

void updateRateValue(uint8_t newValue) {
  if (rate != newValue) {
    rate = newValue;
    Settings::setSetting("ui-w-rate", rate);
    updateWaveParameters();
  }
}

void updateWaveHueValue(uint8_t newValue) {
  if (waveHue != newValue) {
    waveHue = newValue;
    Settings::setSetting("ui-w-whue", waveHue);
    updateWaveParameters();
  }
}

void updateForegroundHueValue(uint8_t newValue) {
  if (foregroundHue != newValue) {
    foregroundHue = newValue;
    Settings::setSetting("ui-w-fhue", foregroundHue);
    updateWaveParameters();
  }
}

void updateBackgroundHueValue(uint8_t newValue) {
  if (backgroundHue != newValue) {
    backgroundHue = newValue;
    Settings::setSetting("ui-w-bhue", backgroundHue);
    updateWaveParameters();
  }
}

Wave::Wave() {
  rate = Settings::getSetting("ui-w-rate", 8);
  waveHue = Settings::getSetting("ui-w-whue", 0);
  foregroundHue = Settings::getSetting("ui-w-fhue", 170);
  backgroundHue = Settings::getSetting("ui-w-bhue", 85);
  this->controls.push_back(new Control::RangeControl(
    "Rate",
    0,
    32,
    rate,
    updateRateValue,
    NULL));
  this->controls.push_back(new Control::RangeControl(
    "Wave Hue",
    0,
    255,
    waveHue,
    updateWaveHueValue,
    NULL));
  this->controls.push_back(new Control::RangeControl(
    "Foreground Hue",
    0,
    255,
    foregroundHue,
    updateForegroundHueValue,
    NULL));
  this->controls.push_back(new Control::RangeControl(
    "Background Hue",
    0,
    255,
    backgroundHue,
    updateBackgroundHueValue,
    NULL));
}

void Wave::updateWave() {
  updateWaveParameters();
}

}  // namespace Wave

#endif  // HAS_UI
