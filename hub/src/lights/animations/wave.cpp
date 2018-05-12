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
#include "./lights/animations/wave.h"
#include "./lights/colorspace.h"
#include "../../config.h"
#include "./codes.h"

namespace Wave {

double step = 0;
byte spacing = 0;
double hueForeground = 0;
double hueBackground = 0;

double brightness = 0;

void WaveAnimation::setBrightness(double newBrightness) {
  brightness = newBrightness;
}

void WaveAnimation::setValues(byte* values) {
  step = (static_cast<double>(values[0]) / 255.0) / 300;
  spacing = values[1];
  hueForeground = static_cast<double>(values[2]) * 360.0 / 255;
  hueBackground = static_cast<double>(values[3]) * 360.0 / 255;
}

void WaveAnimation::updateColors(uint32 commandTime, colorspace::hsv* buffer) {
  uint32 period = 2 / step;
  double periodTime = 2 * PI * (commandTime % period) / period;

  for (int i = 0; i < NUM_PIXELS; i++) {
    double alpha = sin(2 * PI * (i % spacing) / spacing + periodTime);
    if (alpha < 0) {
      alpha = 0;
    }

    colorspace::hsv foregroundHSV;
    foregroundHSV.h = hueForeground;
    foregroundHSV.s = 1;
    foregroundHSV.v = brightness;

    colorspace::rgb foregroundRGB = colorspace::hsv2rgb(foregroundHSV);

    colorspace::hsv backgroundHSV;
    backgroundHSV.h = hueBackground;
    backgroundHSV.s = 1;
    backgroundHSV.v = brightness;

    colorspace::rgb backgroundRGB = colorspace::hsv2rgb(backgroundHSV);

    colorspace::rgb compositedRGB;
    compositedRGB.r = (foregroundRGB.r * alpha + backgroundRGB.r * (1 - alpha));
    compositedRGB.g = (foregroundRGB.g * alpha + backgroundRGB.g * (1 - alpha));
    compositedRGB.b = (foregroundRGB.b * alpha + backgroundRGB.b * (1 - alpha));

    buffer[i] = colorspace::rgb2hsv(compositedRGB);
  }
}

}  // namespace Wave
