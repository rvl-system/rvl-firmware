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
#include "./lights/lights.h"
#include "./codes.h"
#include "../config.h"  // Why does this one single file require ".." but none of the others do?
#include "./state.h"
#include "./event.h"

namespace Lights {

CRGB leds[NUM_PIXELS];
CHSV colors[NUM_PIXELS];

void init() {
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_PIXELS);
  Serial.println("Lights initialized");
}

uint8 calculatePixelValue(State::WaveChannel *wave, uint32 t, uint8 x) {
  return sin8(wave->w_t * t / 100 + wave->w_x * x + wave->phi) * wave->a / 255 + wave->b;
}

void loop() {
  auto settings = State::getSettings();
  FastLED.setBrightness(settings->brightness);
  uint32 t = millis() % (settings->waveSettings.timePeriod * 100) * 255 / settings->waveSettings.timePeriod;
  for (uint16 i = 0; i < NUM_PIXELS; i++) {
    uint8 x = 255 * (i % settings->waveSettings.distancePeriod) / settings->waveSettings.distancePeriod;

    CHSV waveHSV[NUM_WAVES];
    CRGB waveRGB[NUM_WAVES];
    uint8 alphaValues[NUM_WAVES];

    for (uint8 j = 0; j < NUM_WAVES; j++) {
      waveHSV[j].h = calculatePixelValue(&(settings->waveSettings.waves[j].h), t, x);
      waveHSV[j].s = calculatePixelValue(&(settings->waveSettings.waves[j].s), t, x);
      waveHSV[j].v = calculatePixelValue(&(settings->waveSettings.waves[j].v), t, x);
      alphaValues[j] = calculatePixelValue(&(settings->waveSettings.waves[j].a), t, x);
      hsv2rgb_spectrum(waveHSV[j], waveRGB[j]);
    }
    leds[i] = waveRGB[NUM_WAVES - 1];
    for (int8 j = NUM_WAVES - 2; j >= 0; j--) {
      leds[i] = blend(leds[i], waveRGB[j], alphaValues[j]);
    }
  }
  FastLED.show();
}

}  // namespace Lights
