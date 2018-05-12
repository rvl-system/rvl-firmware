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

#define FASTLED_INTERRUPT_RETRY_COUNT 0  // Helps keep LEDs from flickering
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
#include <FastLED.h>
#include "./lights/colorspace.h"
#include "./lights/animation.h"
#include "./lights/lights.h"
#include "./codes.h"
#include "./config.h"
#include "./state.h"
#include "./event.h"

#include "./lights/animations/fade.h"
#include "./lights/animations/pulse.h"
#include "./lights/animations/wave.h"

namespace Lights {

CRGB leds[NUM_PIXELS];

colorspace::hsv colors[NUM_PIXELS];
Codes::Preset::Preset preset = Codes::Preset::Unknown;
byte brightness = 0;
uint32 lastUpdateTime = 0;

Animation::AnimationBase* animations[NUM_PRESETS];

void updateColors();
void displayColors();

void update();

void init() {
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_PIXELS);
  Event::on(Codes::EventType::AnimationChange, update);

  animations[Codes::Preset::Fade] = new Fade::FadeAnimation();
  animations[Codes::Preset::Pulse] = new Pulse::PulseAnimation();
  animations[Codes::Preset::Wave] = new Wave::WaveAnimation();

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

  if (settings->brightness != brightness) {
    Serial.print("Changing brightness to: ");
    Serial.println(settings->brightness);
    brightness = settings->brightness;
    double scaledBrightness = static_cast<double>(settings->brightness) / 255.0;
    for (uint8 i = 0; i < NUM_PRESETS; i++) {
      animations[i]->setBrightness(scaledBrightness);
    }
  }

  if (preset != settings->presetSettings.preset) {
    Serial.print("Changing preset to: ");
    Serial.println(settings->presetSettings.preset);
    preset = (Codes::Preset::Preset)settings->presetSettings.preset;
  }

  if (preset != Codes::Preset::Unknown) {
    animations[preset]->setValues(settings->presetSettings.presetValues[settings->presetSettings.preset]);
  }
}

void updateColors() {
  if (preset != Codes::Preset::Unknown) {
    animations[preset]->updateColors(State::getSettings()->clock, colors);
  }
}

void displayColors() {
  for (uint8 i = 0; i < NUM_PIXELS; i++) {
    colorspace::rgb convertedColor = hsv2rgb(colors[i]);
    leds[i].r = static_cast<int>(convertedColor.r * 255);
    leds[i].g = static_cast<int>(convertedColor.g * 255);
    leds[i].b = static_cast<int>(convertedColor.b * 255);
  }
  FastLED.show();
}

}  // namespace Lights
