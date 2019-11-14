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
#include <rvl-wifi.h>
#include "./lights.h"
#include "./codes.h"
#include "./config.h"
#include "./state.h"
#include "./event.h"

namespace Lights {

CRGB leds[LED_NUM_PIXELS];

void init() {
  FastLED.addLeds<WS2812B, LED_DATA_PIN, LED_COLOR_MODE>(leds, LED_NUM_PIXELS);
  State::getLogger()->info("Lights initialized");
}

uint8_t calculatePixelValue(RVLWaveChannel *wave, uint32_t t, uint8_t x) {
  return sin8(wave->w_t * t / 100 + wave->w_x * x + wave->phi) * wave->a / 255 + wave->b;
}

void loop() {
#ifdef HAS_CLOCK
  if (!State::getTimerState()) {
    FastLED.setBrightness(0);
    FastLED.show();
    return;
  }
#else
  if (!State::getPowerState()) {
    FastLED.setBrightness(0);
    FastLED.show();
    return;
  }
#endif

  auto waveSettings = State::getWaveSettings();
  FastLED.setBrightness(State::getBrightness());
  uint32_t t = RVLGetAnimationClock() % (waveSettings->timePeriod * 100) * 255 / waveSettings->timePeriod;
  for (uint16_t i = 0; i < LED_NUM_PIXELS; i++) {
    uint8_t x = 255 * (i % waveSettings->distancePeriod) / waveSettings->distancePeriod;

    CHSV waveHSV[NUM_WAVES];
    CRGB waveRGB[NUM_WAVES];
    uint8_t alphaValues[NUM_WAVES];

    for (uint8_t j = 0; j < NUM_WAVES; j++) {
      waveHSV[j].h = calculatePixelValue(&(waveSettings->waves[j].h), t, x);
      waveHSV[j].s = calculatePixelValue(&(waveSettings->waves[j].s), t, x);
      waveHSV[j].v = calculatePixelValue(&(waveSettings->waves[j].v), t, x);
      alphaValues[j] = calculatePixelValue(&(waveSettings->waves[j].a), t, x);
      hsv2rgb_spectrum(waveHSV[j], waveRGB[j]);
    }
    leds[i] = waveRGB[NUM_WAVES - 1];
    for (int8_t j = NUM_WAVES - 2; j >= 0; j--) {
      leds[i] = blend(leds[i], waveRGB[j], alphaValues[j]);
    }
  }
  FastLED.show();
}

}  // namespace Lights
