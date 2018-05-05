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

#ifndef LIGHTS_COLORSPACE_H_
#define LIGHTS_COLORSPACE_H_

namespace colorspace {

typedef struct {
  double r;       // percent
  double g;       // percent
  double b;       // percent
} rgb;

typedef struct {
  double h;       // angle in degrees
  double s;       // percent
  double v;       // percent
} hsv;

hsv rgb2hsv(rgb in);
rgb hsv2rgb(hsv in);

}  // namespace colorspace

#endif  // LIGHTS_COLORSPACE_H_
