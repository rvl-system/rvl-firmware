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
#include "common/codes.h"
#include "config.h"
#include "lights.h"

#include "animations/fade.h"
#include "animations/pulse.h"

Adafruit_DotStar strip = Adafruit_DotStar(NUM_PIXELS, DATA_PIN, CLOCK_PIN, DOTSTAR_BRG);

hsv colors[NUM_PIXELS];
Codes::Preset::Preset preset;
byte brightness = 0;

void updateColors();
void displayColors();

void Lights::setup() {
  strip.begin();
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    rgb converted_color = hsv2rgb(colors[i]);
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
  Fade::setBuffer(colors);
  Pulse::setBuffer(colors);

  Serial.println("Lights initialized");
}

void Lights::loop() {
  updateColors();
  displayColors();
}

void Lights::setPreset(Codes::Preset::Preset newPreset) {
  preset = newPreset;
  switch (preset) {
    case Codes::Preset::Fade:
      Fade::initColors();
      Fade::setValue(Codes::FadeValue::Rate, FADE_DEFAULT_RATE);
      break;
    case Codes::Preset::Pulse:
      Pulse::initColors();
      Pulse::setValue(Codes::PulseValue::Rate, PULSE_DEFAULT_RATE);
      Pulse::setValue(Codes::PulseValue::Hue, PULSE_DEFAULT_HUE);
      Pulse::setValue(Codes::PulseValue::Saturation, PULSE_DEFAULT_SATURATION);
      break;
  }
  displayColors();
}

void Lights::setValue(byte type, byte value) {
  switch (preset) {
    case Codes::Preset::Fade:
      switch (type) {
        case Codes::FadeValue::Rate:
          Fade::setValue(Codes::FadeValue::Rate, value);
          break;
        default:
          Serial.println("Attempted to set invalid Fade Value");
          break;
      }
      break;
    case Codes::Preset::Pulse:
      switch(type) {
        case Codes::PulseValue::Rate:
          Pulse::setValue(Codes::PulseValue::Rate, value);
          break;
        case Codes::PulseValue::Hue:
          Pulse::setValue(Codes::PulseValue::Hue, value);
          break;
        case Codes::PulseValue::Saturation:
          Pulse::setValue(Codes::PulseValue::Saturation, value);
          break;
        default:
          Serial.println("Attempted to set invalid Pulse Value");
          break;
      }
      break;
  }
}

void Lights::setBrightness(byte newBrightness) {
  double scaledBrightness = (double)newBrightness / 255.0;
  Fade::setBrightness(scaledBrightness);
  Pulse::setBrightness(scaledBrightness);
}

void updateColors() {
  switch (preset) {
    case Codes::Preset::Fade:
      Fade::updateColors();
      break;
    case Codes::Preset::Pulse:
      Pulse::updateColors();
      break;
  }
}

void displayColors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    rgb converted_color = hsv2rgb(colors[i]);
    strip.setPixelColor(i,
      (int)(converted_color.r * 255),
      (int)(converted_color.g * 255),
      (int)(converted_color.b * 255)
    );
  }
  strip.show();
}
