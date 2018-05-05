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
#include "./colorspace.h"
#include "./codes.h"
#include "./animation.h"
#include "./config.h"
#include "./lights.h"
#include "./state.h"
#include "./events.h"

#include "./animations/fade.h"
#include "./animations/pulse.h"
#include "./animations/wave.h"

namespace Lights {

Adafruit_DotStar strip = Adafruit_DotStar(NUM_PIXELS, DATA_PIN, CLOCK_PIN, DOTSTAR_BRG);

colorspace::hsv colors[NUM_PIXELS];
Codes::Preset::Preset preset = Codes::Preset::Unknown;
byte brightness = 0;
uint32 commandTime = 0;
uint32 lastUpdateTime = 0;

Animation::AnimationBase* animations[NUM_PRESETS];

void updateColors();
void displayColors();

void update();

class LightsStateListener : public Events::EventListenerInterface {
 public:
  void onEvent() {
    update();
  }
};

void init() {
  Events::on(Codes::EventTypes::AnimationChange, new LightsStateListener());

  animations[Codes::Preset::Fade] = new Fade::FadeAnimation();
  animations[Codes::Preset::Pulse] = new Pulse::PulseAnimation();
  animations[Codes::Preset::Wave] = new Wave::WaveAnimation();

  strip.begin();
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    colorspace::rgb converted_color = hsv2rgb(colors[i]);
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
  commandTime = settings->commandTime + 500;

  if (settings->brightness != brightness) {
    Serial.print("Changing brightness to: ");
    Serial.println(settings->brightness);
    brightness = settings->brightness;
    double scaledBrightness = static_cast<double>(settings->brightness) / 255.0;
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
    uint32 adjustedCommandTime = (millis() - lastUpdateTime) + commandTime;
    animations[preset]->updateColors(adjustedCommandTime, colors);
  }
}

void displayColors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    colorspace::rgb convertedColor = hsv2rgb(colors[i]);
    strip.setPixelColor(i,
      static_cast<int>(convertedColor.r * 255),
      static_cast<int>(convertedColor.g * 255),
      static_cast<int>(convertedColor.b * 255));
  }
  strip.show();
}

}  // namespace Lights
