/*
Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>

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

#include <FastLED.h>
#include <math.h>
#include "./lights/animations/wave.h"
#include "../../config.h"
#include "./codes.h"

namespace Wave {

double rate = 0;
uint8 foregroundHue = 0;
uint8 foregroundSaturation = 0;
uint8 backgroundHue = 0;
uint8 backgroundSaturation = 0;

uint8 brightness = 0;

#define SPACING 16

void WaveAnimation::setBrightness(uint8 newBrightness) {
  brightness = newBrightness;
}

void WaveAnimation::setValues(uint8* values) {
  rate = (static_cast<double>(values[values[Codes::WavePresetValues::Rate]]) / 255.0) / 300;
  foregroundHue = values[Codes::WavePresetValues::ForegroundHue];
  foregroundSaturation = values[Codes::WavePresetValues::ForegroundSaturation];
  backgroundHue = values[Codes::WavePresetValues::BackgroundHue];
  backgroundSaturation = values[Codes::WavePresetValues::BackgroundSaturation];
}

void WaveAnimation::updateColors(uint32 commandTime, CHSV* buffer) {
  uint32 period = 2 / rate;
  double periodTime = 2 * PI * (commandTime % period) / period;

  for (uint16 i = 0; i < NUM_PIXELS; i++) {
    // TODO(nebrius): convert to sin8 or other FastLED method
    double alpha = sin(2 * PI * (i % SPACING) / SPACING + periodTime);
    if (alpha < 0) {
      alpha = 0;
    }

    CHSV foregroundHSV;
    foregroundHSV.h = foregroundHue;
    foregroundHSV.s = foregroundSaturation;
    foregroundHSV.v = brightness;

    CHSV backgroundHSV;
    backgroundHSV.h = backgroundHue;
    backgroundHSV.s = backgroundSaturation;
    backgroundHSV.v = brightness;

    buffer[i] = blend(foregroundHSV, backgroundHSV, alpha * 255);
  }
}

}  // namespace Wave
