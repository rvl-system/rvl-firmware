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
