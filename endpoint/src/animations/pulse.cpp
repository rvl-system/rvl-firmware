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

#include "pulse.h"
#include "colorspace.h"
#include "config.h"
#include "common/codes.h"

#define COLOR_DIFFERENCE 40

namespace Pulse {

  double pulseStep = 0;
  double hue = 0;
  double saturation = 0;

  double brightness = 0;

  void PulseAnimation::setBrightness(double newBrightness) {
    brightness = newBrightness;
  }

  void PulseAnimation::setValues(byte* values) {
    pulseStep = ((double)values[0] / 255.0) / 300;
    hue = (double)values[1] * 360.0 / 255;
    saturation = (double)values[2] / 255;
  }

  void PulseAnimation::updateColors(uint32_t commandTime, hsv* buffer) {

    uint32_t period = 2 / pulseStep;
    uint32_t periodTime = commandTime % period;

    double currentBrightness;
    if (periodTime < period / 2) {
      currentBrightness = (double)periodTime / ((double)period / 2.0);
    } else {
      currentBrightness = (double)(period - periodTime) / ((double)period / 2.0);
    }

    for (int i = 0; i < NUM_PIXELS; i++) {
      buffer[i].v = currentBrightness * brightness;
      buffer[i].h = hue;
      buffer[i].s = saturation;
    }
  }

}
