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

#define NUM_LOOP_SAMPLES 60
uint8_t loopTimes[NUM_LOOP_SAMPLES];
uint8_t loopIndex = 0;

void animationLoop() {
  uint32_t startTime = millis();

  if (!rvl::getPowerState()) {
    FastLED.setBrightness(0);
    FastLED.show();
    return;
  }

  auto* waveSettings = rvl::getWaveSettings();
  FastLED.setBrightness(rvl::getBrightness());
  uint32_t t = rvl::getAnimationClock() % (waveSettings->timePeriod * 100) *
      255 / waveSettings->timePeriod;
  for (uint16_t i = 0; i < LED_NUM_PIXELS; i++) {
    uint8_t x =
        255 * (i % waveSettings->distancePeriod) / waveSettings->distancePeriod;

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

  uint32_t now = millis();
  if (loopIndex < NUM_LOOP_SAMPLES) {
    loopTimes[loopIndex++] = now - startTime;
  }
  if (now - startTime > UPDATE_RATE) {
    delay(1);
  } else {
    delay(UPDATE_RATE - (millis() - startTime));
  }
}

void animationLoopRunner(void* parameters) {
  while (true) {
    animationLoop();
  }
}

void startAnimationLoop() {
  // Temporarily disabled multi-core rendering due to
  // https://github.com/rvl-system/rvl-firmware/issues/13
#ifdef FALSE // ESP32
  xTaskCreatePinnedToCore(
      animationLoopRunner, "animationLoopRunner", 8192, NULL, 2, NULL, 0);
#endif
}

bool animationLoopStarted = false;
void loop() {
  // Temporarily disabled multi-core rendering due to
  // https://github.com/rvl-system/rvl-firmware/issues/13
#ifdef FALSE // ESP32
  if (!animationLoopStarted) {
    animationLoopStarted = true;
    startAnimationLoop();
  }
#else
  animationLoop();
#endif
  if (loopIndex == NUM_LOOP_SAMPLES) {
    loopIndex = 0;
    uint16_t sum = 0;
    uint8_t min = 255;
    uint8_t max = 0;
    for (uint8_t i = 0; i < NUM_LOOP_SAMPLES; i++) {
      sum += loopTimes[i];
      if (loopTimes[i] < min) {
        min = loopTimes[i];
      }
      if (loopTimes[i] > max) {
        max = loopTimes[i];
      }
    }
    rvl::info("LED render stats: Avg=%d Min=%d Max=%d", sum / NUM_LOOP_SAMPLES,
        min, max);
  }
}

} // namespace Lights
