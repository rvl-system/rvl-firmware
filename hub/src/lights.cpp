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
#include "codes.h"
#include "animation.h"
#include "config.h"
#include "lights.h"
#include "state.h"

#include "animations/fade.h"
#include "animations/pulse.h"
#include "animations/wave.h"

namespace Lights {

  Adafruit_DotStar strip = Adafruit_DotStar(NUM_PIXELS, DATA_PIN, CLOCK_PIN, DOTSTAR_BRG);

  hsv colors[NUM_PIXELS];
  Codes::Preset::Preset preset = Codes::Preset::Unknown;
  byte brightness = 0;
  uint32_t commandTime = 0;
  unsigned long lastUpdateTime = 0;

  Animation::AnimationBase* animations[NUM_PRESETS];

  void updateColors();
  void displayColors();

  void init() {

    animations[Codes::Preset::Fade] = new Fade::FadeAnimation();
    animations[Codes::Preset::Pulse] = new Pulse::PulseAnimation();
    animations[Codes::Preset::Wave] = new Wave::WaveAnimation();

    strip.begin();
    for (unsigned int i = 0; i < NUM_PIXELS; i++) {
      rgb converted_color = hsv2rgb(colors[i]);
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();

    update();

    Serial.println("Lights initialized");
  }

  void loop() {
    updateColors();
    displayColors();
  }

  void update() {
    State::Settings* settings = State::getSettings();

    lastUpdateTime = millis();

    Serial.print("Setting command time to: ");
    Serial.println(settings->commandTime);
    commandTime = settings->commandTime;

    if (settings->brightness != brightness) {
      Serial.print("Changing brightness to: ");
      Serial.println(settings->brightness);
      brightness = settings->brightness;
      double scaledBrightness = (double)settings->brightness / 255.0;
      for (int i = 0; i < NUM_PRESETS; i++) {
        animations[i]->setBrightness(scaledBrightness);
      }
    }

    if (preset != settings->preset) {
      Serial.print("Changing preset to: ");
      Serial.println(settings->preset);
      preset = (Codes::Preset::Preset)settings->preset;
    }

    if (preset != Codes::Preset::Unknown) {
      animations[preset]->setValues(settings->presetValues[settings->preset]);
    }
  }

  void updateColors() {
    if (preset != Codes::Preset::Unknown) {
      uint32_t adjustedCommandTime = (millis() - lastUpdateTime) + commandTime;
      animations[preset]->updateColors(adjustedCommandTime, colors);
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

}
