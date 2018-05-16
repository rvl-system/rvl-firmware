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

double step = 0;
uint8 spacing = 0;
double hueForeground = 0;
double hueBackground = 0;

uint8 brightness = 0;

void WaveAnimation::setBrightness(uint8 newBrightness) {
  brightness = newBrightness;
}

void WaveAnimation::setValues(uint8* values) {
  step = (static_cast<double>(values[0]) / 255.0) / 300;
  spacing = values[1];
  hueForeground = static_cast<double>(values[2]) * 360.0 / 255;
  hueBackground = static_cast<double>(values[3]) * 360.0 / 255;
}

void WaveAnimation::updateColors(uint32 commandTime, CHSV* buffer) {
  uint32 period = 2 / step;
  double periodTime = 2 * PI * (commandTime % period) / period;

  for (uint16 i = 0; i < NUM_PIXELS; i++) {
    double alpha = sin(2 * PI * (i % spacing) / spacing + periodTime);
    if (alpha < 0) {
      alpha = 0;
    }

    CHSV foregroundHSV;
    foregroundHSV.h = hueForeground;
    foregroundHSV.s = 255;
    foregroundHSV.v = brightness;

    CHSV backgroundHSV;
    backgroundHSV.h = hueBackground;
    backgroundHSV.s = 255;
    backgroundHSV.v = brightness;

    buffer[i] = blend(foregroundHSV, backgroundHSV, alpha);
  }
}

}  // namespace Wave
