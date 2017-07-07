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
Codes::Preset::Preset preset = Codes::Preset::Unknown;
byte brightness = 0;
uint32_t commandTime = 0;
unsigned long lastUpdateTime = 0;

void updateColors();
void displayColors();

void Lights::setup() {
  strip.begin();
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    rgb converted_color = hsv2rgb(colors[i]);
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();

  Serial.println("Lights initialized");
}

void Lights::loop() {
  updateColors();
  displayColors();
}

void Lights::update(uint32_t newCommandTime, byte newBrightness, Codes::Preset::Preset newPreset, byte* newPresetValues) {

  lastUpdateTime = millis();

  Serial.print("Setting command time to: ");
  Serial.println(newCommandTime);
  commandTime = newCommandTime;

  if (newBrightness != brightness) {
    Serial.print("Changing brightness to: ");
    Serial.println(newBrightness);
    brightness = newBrightness;
    double scaledBrightness = (double)newBrightness / 255.0;
    Fade::setBrightness(scaledBrightness);
    Pulse::setBrightness(scaledBrightness);
  }

  if (preset != newPreset) {
    Serial.print("Changing preset to: ");
    Serial.println(newPreset);
    preset = newPreset;
  }

  switch (preset) {
    case Codes::Preset::Fade:
      Fade::setValues(newPresetValues);
      break;
    case Codes::Preset::Pulse:
      Pulse::setValues(newPresetValues);
      break;
  }
}

void updateColors() {
  uint32_t adjustedCommandTime = (millis() - lastUpdateTime) + commandTime;
  switch (preset) {
    case Codes::Preset::Fade:
      Fade::updateColors(adjustedCommandTime, colors);
      break;
    case Codes::Preset::Pulse:
      Pulse::updateColors(adjustedCommandTime, colors);
      break;
  }
}

void displayColors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    rgb convertedColor = hsv2rgb(colors[i]);
    strip.setPixelColor(i,
      (int)(convertedColor.r * 255),
      (int)(convertedColor.g * 255),
      (int)(convertedColor.b * 255)
    );
  }
  strip.show();
}
