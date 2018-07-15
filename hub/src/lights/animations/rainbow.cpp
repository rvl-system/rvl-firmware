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
#include "./lights/animations/rainbow.h"
#include "../../config.h"
#include "./codes.h"

#define COLOR_DIFFERENCE 40

namespace Rainbow {

double rate = 0;
uint8 brightness = 0;

void RainbowAnimation::setBrightness(uint8 newBrightness) {
  brightness = newBrightness;
}

void RainbowAnimation::setValues(uint8* values) {
  rate = static_cast<double>(values[Codes::RainbowPresetValues::Rate]) / 1280.0;
}

void RainbowAnimation::updateColors(uint32 commandTime, CHSV* buffer) {
  for (uint16 i = 0; i < NUM_PIXELS; i++) {
    buffer[i].h = (static_cast<uint32>(commandTime * rate) + COLOR_DIFFERENCE * i) % 255;
    buffer[i].s = 255;
    buffer[i].v = brightness;
  }
}

}  // namespace Rainbow