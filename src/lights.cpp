/*
Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>

This file is part of RVL Firmware.

RVL Firmware is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RVL Firmware is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RVL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

// ESP8266 specific stuff
#ifdef ESP8266
#define FASTLED_INTERRUPT_RETRY_COUNT 0 // Helps keep LEDs from flickering
#define FASTLED_ESP8266_RAW_PIN_ORDER
#endif

#include "./lights.hpp"
#include "./codes.hpp"
#include "./config.hpp"
#include "./state.hpp"
#include <Arduino.h>
#include <FastLED.h>
#include <rvl-wifi.hpp>

namespace Lights {

CRGB leds[LED_NUM_PIXELS];

void init() {
  FastLED.addLeds<WS2812B, LED_DATA_PIN, LED_COLOR_MODE>(leds, LED_NUM_PIXELS);
  rvl::info("Lights initialized");
}

uint8_t calculatePixelValue(RVLWaveChannel* wave, uint32_t t, uint8_t x) {
  return sin8(wave->w_t * t / 100 + wave->w_x * x + wave->phi) * wave->a / 255 +
      wave->b;
}

void loop() {
  if (!rvl::getPowerState()) {
    FastLED.setBrightness(0);
    FastLED.show();
    return;
  }

  RVLWaveSettings waveSettings;
  rvl::lockState();
  memcpy(&waveSettings, rvl::getWaveSettings(), sizeof(RVLWaveSettings));
  FastLED.setBrightness(rvl::getBrightness());
  auto animationClock = rvl::getAnimationClock();
  rvl::freeState();

  uint32_t t = animationClock % (waveSettings.timePeriod * 100) * 255 /
      waveSettings.timePeriod;
  for (const auto& segment : segments) {
    for (uint16_t i = segment.start; i <= segment.end; i++) {
      uint16_t normalizedIndex = 0;
      if (segment.reverse) {
        normalizedIndex = (segment.end - i) + segment.offset;
      } else {
        normalizedIndex = (i - segment.start) + segment.offset;
      }
      uint8_t x = 255 * (normalizedIndex % waveSettings.distancePeriod) /
          waveSettings.distancePeriod;

      CHSV waveHSV[NUM_WAVES];
      CRGB waveRGB[NUM_WAVES];
      uint8_t alphaValues[NUM_WAVES];

      for (uint8_t j = 0; j < NUM_WAVES; j++) {
        waveHSV[j].h = calculatePixelValue(&(waveSettings.waves[j].h), t, x);
        waveHSV[j].s = calculatePixelValue(&(waveSettings.waves[j].s), t, x);
        waveHSV[j].v = calculatePixelValue(&(waveSettings.waves[j].v), t, x);
        alphaValues[j] = calculatePixelValue(&(waveSettings.waves[j].a), t, x);
        hsv2rgb_spectrum(waveHSV[j], waveRGB[j]);
      }
      leds[i] = waveRGB[NUM_WAVES - 1];
      for (int8_t j = NUM_WAVES - 2; j >= 0; j--) {
        leds[i] = blend(leds[i], waveRGB[j], alphaValues[j]);
      }
    }
  }

  FastLED.show();
}

} // namespace Lights
