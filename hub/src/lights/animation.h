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

#ifndef LIGHTS_ANIMATION_H_
#define LIGHTS_ANIMATION_H_

#include <Arduino.h>
#include <FastLED.h>

namespace Animation {

class AnimationBase {
 public:
  virtual void setBrightness(uint8 newBrightness) = 0;
  virtual void setValues(uint8* values) = 0;
  virtual void updateColors(uint32 commandTime, CHSV* buffer) = 0;
};

}

#endif  // LIGHTS_ANIMATION_H_
