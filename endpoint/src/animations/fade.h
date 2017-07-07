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

#ifndef FADE_HEADER
#define FADE_HEADER

#include <Arduino.h>
#include "colorspace.h"
#include "common/codes.h"

#define FADE_DEFAULT_RATE 128

namespace Fade {

  void setBrightness(double newBrightness);
  void setValues(byte* values);
  void updateColors(uint32_t commandTime, hsv* buffer);

}

#endif
