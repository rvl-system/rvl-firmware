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
  double currentBrightness = 0;
  unsigned char currentDirection = 0;

  void setBrightness(double newBrightness) {
    brightness = newBrightness;
  }

  void setValues(byte* values) {
    pulseStep = ((double)values[0] / 255.0) / 100;
    hue = (double)values[1] * 360.0 / 255;
    saturation = (double)values[2] / 255;
  }

  void initColors(hsv* buffer) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      buffer[i].h = 0;
      buffer[i].s = 1;
      buffer[i].v = 0;
    }
    currentBrightness = 0;
    currentDirection = 0;
  }

  void updateColors(uint32_t commandTime, hsv* buffer) {
    if (currentDirection) {
      currentBrightness += pulseStep;
      if (currentBrightness >= 1.0) {
        currentDirection = 0;
        currentBrightness = 1;
      }
    } else {
      currentBrightness -= pulseStep;
      if (currentBrightness <= 0) {
        currentDirection = 1;
        currentBrightness = 0;
      }
    }
    for (int i = 0; i < NUM_PIXELS; i++) {
      buffer[i].v = currentBrightness * brightness;
      buffer[i].h = hue;
      buffer[i].s = saturation;
    }
  }

}
