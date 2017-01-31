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

#define COLOR_DIFFERENCE 40

hsv* pulse_buffer;

double pulse_step = 0;

double pulse_max_brightness = 0;
double current_brightness = 0;
unsigned char current_direction = 0;

void pulse_set_buffer(hsv* new_buffer) {
  pulse_buffer = new_buffer;
}

void pulse_lights_set_brightness(double new_brightness) {
  pulse_max_brightness = new_brightness;
}

void pulse_set_value(unsigned char type, unsigned char value) {
  switch(type) {
    case PULSE_RATE:
      pulse_step = ((double)value / 255.0) / 100;
      break;
    case PULSE_HUE:
      for (unsigned int i = 0; i < NUM_PIXELS; i++) {
        pulse_buffer[i].h = (double)value * 360.0 / 255;
      }
      break;
    case PULSE_SATURATION:
      for (unsigned int i = 0; i < NUM_PIXELS; i++) {
        pulse_buffer[i].s = (double)value / 255;
      }
      break;
  }
}

void pulse_init_colors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    pulse_buffer[i].h = 0;
    pulse_buffer[i].s = 1;
    pulse_buffer[i].v = 0;
  }
  current_brightness = 0;
  current_direction = 0;
}

void pulse_update_colors() {
  if (current_direction) {
    current_brightness += pulse_step;
    if (current_brightness >= 1.0) {
      current_direction = 0;
      current_brightness = 1;
    }
  } else {
    current_brightness -= pulse_step;
    if (current_brightness <= 0) {
      current_direction = 1;
      current_brightness = 0;
    }
  }
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    pulse_buffer[i].v = current_brightness * pulse_max_brightness;
  }
}
