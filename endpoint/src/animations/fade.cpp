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

#include "animations/fade.h"
#include "colorspace.h"
#include "config.h"
#include "common/codes.h"

#define COLOR_DIFFERENCE 40

hsv* fadeBuffer;

double fadeColorStep = 0;
double fadeMaxBrightness = 0;

void Fade::setBuffer(hsv* newBuffer) {
  fadeBuffer = newBuffer;
}

void Fade::setBrightness(double newBrightness) {
  fadeMaxBrightness = newBrightness;
}

void Fade::setValue(Codes::FadeValue::FadeValue type, byte value) {
  switch(type) {
    case Codes::FadeValue::Rate:
      fadeColorStep = (double)value / 1280.0;
      break;
  }
}

void Fade::initColors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    fadeBuffer[i].h = COLOR_DIFFERENCE * i; // angle
    fadeBuffer[i].s = 1;
    fadeBuffer[i].v = fadeMaxBrightness;
  }
}

void Fade::updateColors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    fadeBuffer[i].h += fadeColorStep;
    if (fadeBuffer[i].h >= 360.0) {
      fadeBuffer[i].h = 0;
    }
    fadeBuffer[i].v = fadeMaxBrightness;
  }
}
