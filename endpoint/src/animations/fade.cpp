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

#define COLOR_DIFFERENCE 40

hsv* fade_buffer;

double fade_color_step = 0;

void fade_set_buffer(hsv* new_buffer) {
  fade_buffer = new_buffer;
}

void fade_set_value(unsigned char type, unsigned char value) {
  switch(type) {
    case FADE_RATE:
      fade_color_step = (double)value / 1280.0;
      break;
  }
}

void fade_init_colors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    fade_buffer[i].h = COLOR_DIFFERENCE * i; // angle
    fade_buffer[i].s = 1;
    fade_buffer[i].v = BRIGHTNESS;
  }
}

void fade_update_colors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    fade_buffer[i].h += fade_color_step;
    if (fade_buffer[i].h >= 360.0) {
      fade_buffer[i].h = 0;
    }
  }
}
