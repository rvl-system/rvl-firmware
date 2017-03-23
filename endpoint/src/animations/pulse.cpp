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

hsv* pulseBuffer;

double pulseStep = 0;

double pulseMaxBrightness = 0;
double currentBrightness = 0;
unsigned char currentDirection = 0;

void Pulse::setBuffer(hsv* newBuffer) {
  pulseBuffer = newBuffer;
}

void Pulse::setBrightness(double newBrightness) {
  pulseMaxBrightness = newBrightness;
}

void Pulse::setValue(Codes::PulseValue::PulseValue type, byte value) {
  switch(type) {
    case Codes::PulseValue::Rate:
      pulseStep = ((double)value / 255.0) / 100;
      break;
    case Codes::PulseValue::Hue:
      for (unsigned int i = 0; i < NUM_PIXELS; i++) {
        pulseBuffer[i].h = (double)value * 360.0 / 255;
      }
      break;
    case Codes::PulseValue::Saturation:
      for (unsigned int i = 0; i < NUM_PIXELS; i++) {
        pulseBuffer[i].s = (double)value / 255;
      }
      break;
  }
}

void Pulse::initColors() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pulseBuffer[i].h = 0;
    pulseBuffer[i].s = 1;
    pulseBuffer[i].v = 0;
  }
  currentBrightness = 0;
  currentDirection = 0;
}

void Pulse::updateColors() {
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
    pulseBuffer[i].v = currentBrightness * pulseMaxBrightness;
  }
}
