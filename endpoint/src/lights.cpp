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
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    rgb converted_color = hsv2rgb(colors[i]);
    strip.setPixelColor(i, 0, 0, 0);
  }
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
