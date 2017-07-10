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

#include <math.h>
#include "wave.h"
#include "colorspace.h"
#include "config.h"
#include "common/codes.h"

#define MIN_MAGNITUDE 0

namespace Wave {

  double step = 0;
  byte spacing = 0;
  double hue = 0;

  double brightness = 0;

  void WaveAnimation::setBrightness(double newBrightness) {
    brightness = newBrightness;
  }

  void WaveAnimation::setValues(byte* values) {
    step = ((double)values[0] / 255.0) / 300;
    spacing = values[1];
    hue = (double)values[2] * 360.0 / 255;
  }

  void WaveAnimation::updateColors(uint32_t commandTime, hsv* buffer) {

    uint32_t period = 2 / step;
    double periodTime = 2 * PI * (commandTime % period) / period;

    for (int i = 0; i < NUM_PIXELS; i++) {
      double magnitude = sin(2 * PI * (i % spacing) / spacing + periodTime);
      buffer[i].h = hue;
      buffer[i].s = 1.0;
      buffer[i].v = (magnitude < MIN_MAGNITUDE ? MIN_MAGNITUDE : magnitude) * brightness;
    }
  }

}
