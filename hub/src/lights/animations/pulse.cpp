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
#include "./lights/animations/pulse.h"
#include "../../config.h"
#include "./codes.h"

namespace Pulse {

double step = 0;
uint8 hue = 0;
uint8 saturation = 0;

uint8 brightness = 0;

void PulseAnimation::setBrightness(uint8 newBrightness) {
  brightness = newBrightness;
}

void PulseAnimation::setValues(uint8* values) {
  step = (static_cast<double>(values[0]) / 255.0) / 300;
  hue = values[1];
  saturation = values[2];
}

void PulseAnimation::updateColors(uint32 commandTime, CHSV* buffer) {
  uint32 period = 2 / step;
  uint32 periodTime = commandTime % period;

  double currentBrightness;
  if (periodTime < period / 2) {
    currentBrightness = static_cast<double>(periodTime) / (static_cast<double>(period) / 2.0);
  } else {
    currentBrightness = static_cast<double>(period - periodTime) / (static_cast<double>(period) / 2.0);
  }

  for (uint16 i = 0; i < NUM_PIXELS; i++) {
    buffer[i].h = hue;
    buffer[i].s = saturation;
    buffer[i].v = currentBrightness * brightness;
  }
}

}  // namespace Pulse
