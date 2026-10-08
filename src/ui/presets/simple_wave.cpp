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

#include "./ui/presets/simple_wave.hpp"
#include "./settings.hpp"
#include <Arduino.h>
#include <rvl.hpp>
#include <vector>

// Wave without its foreground layer, so the strip's average color holds still
// while the pattern moves. The same bytes as rvl-node's
// createSimpleWaveParameters at full brightness
namespace SimpleWave {

uint8_t rate;
uint8_t spacing;
uint8_t waveHue;
uint8_t backgroundHue;

void updateParametricSettings() {
  RVLParametricSettings newSettings;

  // Wave layer. Spacing is how many waves fit in a distance period, so a
  // higher value makes narrower waves
  newSettings.layers[0].h.b = waveHue;
  newSettings.layers[0].s.b = 255;
  newSettings.layers[0].v.b = 255;
  newSettings.layers[0].a.a = 255;
  newSettings.layers[0].a.w_t = rate;
  newSettings.layers[0].a.w_x = spacing;

  // Background layer
  newSettings.layers[2].h.b = backgroundHue;
  newSettings.layers[2].s.b = 255;
  newSettings.layers[2].v.b = 255;
  newSettings.layers[2].a.a = 255;

  rvl::setParametricSettings(&newSettings);
}

void updateRateValue(uint8_t newValue) {
  if (rate != newValue) {
    rate = newValue;
    Settings::setSetting("ui-sw-rate", rate);
    updateParametricSettings();
  }
}

void updateSpacingValue(uint8_t newValue) {
  if (spacing != newValue) {
    spacing = newValue;
    Settings::setSetting("ui-sw-space", spacing);
    updateParametricSettings();
  }
}

void updateWaveHueValue(uint8_t newValue) {
  if (waveHue != newValue) {
    waveHue = newValue;
    Settings::setSetting("ui-sw-whue", waveHue);
    updateParametricSettings();
  }
}

void updateBackgroundHueValue(uint8_t newValue) {
  if (backgroundHue != newValue) {
    backgroundHue = newValue;
    Settings::setSetting("ui-sw-bhue", backgroundHue);
    updateParametricSettings();
  }
}

SimpleWave::SimpleWave() {
  rate = Settings::getSetting("ui-sw-rate", 8);
  spacing = Settings::getSetting("ui-sw-space", 2);
  waveHue = Settings::getSetting("ui-sw-whue", 0);
  backgroundHue = Settings::getSetting("ui-sw-bhue", 85);
  this->controls.push_back(
      new Control::RangeControl("Rate", 0, 32, rate, updateRateValue, NULL));
  this->controls.push_back(new Control::RangeControl(
      "Spacing", 1, 16, spacing, updateSpacingValue, NULL));
  this->controls.push_back(new Control::RangeControl(
      "Wave Hue", 0, 255, waveHue, updateWaveHueValue, NULL));
  this->controls.push_back(new Control::RangeControl(
      "Background Hue", 0, 255, backgroundHue, updateBackgroundHueValue, NULL));
}

void SimpleWave::updateParameters() {
  updateParametricSettings();
}

} // namespace SimpleWave

#endif // HAS_UI
