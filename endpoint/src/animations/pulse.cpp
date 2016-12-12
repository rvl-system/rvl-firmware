/*
Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "pulse.h"
#include "colorspace.h"
#include "config.h"

#define COLOR_DIFFERENCE 40

hsv* pulse_buffer;

double pulse_rate = 0;

double current_brightness = 0;
unsigned char current_direction = 0;

void pulse_set_buffer(hsv* new_buffer) {
  pulse_buffer = new_buffer;
}

void pulse_set_value(unsigned char type, unsigned char value) {
  switch(type) {
    case PULSE_RATE:
      pulse_rate = (double)value / 64000.0;
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
    current_brightness += pulse_rate;
    if (current_brightness >= 1.0) {
      current_direction = 0;
      current_brightness = 1;
    }
  } else {
    current_brightness -= pulse_rate;
    if (current_brightness <= 0) {
      current_direction = 1;
      current_brightness = 0;
    }
  }
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    pulse_buffer[i].v = current_brightness * BRIGHTNESS;
  }
}
