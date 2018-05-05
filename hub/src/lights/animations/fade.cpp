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

#include "./lights/animations/fade.h"
#include "./lights/colorspace.h"
#include "./config.h"
#include "./codes.h"

#define COLOR_DIFFERENCE 40

#define NUM_PIXELS 8

namespace Fade {

double step = 0;
double brightness = 0;

void FadeAnimation::setBrightness(double newBrightness) {
  brightness = newBrightness;
}

void FadeAnimation::setValues(byte* values) {
  step = static_cast<double>(values[0]) / 1280.0;
}

void FadeAnimation::updateColors(uint32 commandTime, colorspace::hsv* buffer) {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    buffer[i].h = (static_cast<int>(commandTime * step) + COLOR_DIFFERENCE * i) % 360;
    buffer[i].s = 1;
    buffer[i].v = brightness;
  }
}

}  // namespace Fade
