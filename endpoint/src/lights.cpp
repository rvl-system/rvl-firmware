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

#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include "colorspace.h"
#include "config.h"
#include "lights.h"

#include "animations/fade.h"
#include "animations/pulse.h"

Adafruit_DotStar strip = Adafruit_DotStar(NUM_PIXELS, DATA_PIN, CLOCK_PIN, DOTSTAR_BRG);

hsv colors[NUM_PIXELS];
unsigned char preset;

void update_colors();
void display_colors();

void lights_setup() {
  strip.begin();
  strip.show();
  fade_set_buffer(colors);
  pulse_set_buffer(colors);
}

void lights_loop() {
  update_colors();
  display_colors();
}

void lights_set_preset(unsigned char new_preset) {
  preset = new_preset;
  switch (preset) {
    case FADE_PRESET:
      fade_init_colors();
      fade_set_value(FADE_RATE, FADE_DEFAULT_RATE);
      break;
    case PULSE_PRESET:
      pulse_init_colors();
      pulse_set_value(PULSE_RATE, PULSE_DEFAULT_RATE);
      pulse_set_value(PULSE_HUE, PULSE_DEFAULT_HUE);
      pulse_set_value(PULSE_SATURATION, PULSE_DEFAULT_SATURATION);
      break;
  }
  display_colors();
}

void lights_set_value(unsigned char type, unsigned char value) {
  switch (preset) {
    case FADE_PRESET:
      fade_set_value(type, value);
      break;
    case PULSE_PRESET:
      pulse_set_value(type, value);
      break;
  }
}

void update_colors() {
  switch (preset) {
    case FADE_PRESET:
      fade_update_colors();
      break;
    case PULSE_PRESET:
      pulse_update_colors();
      break;
  }
}

void display_colors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    rgb converted_color = hsv2rgb(colors[i]);
    strip.setPixelColor(i, (int)(converted_color.r * 255), (int)(converted_color.g * 255), (int)(converted_color.b * 255));
  }
  strip.show();
}
