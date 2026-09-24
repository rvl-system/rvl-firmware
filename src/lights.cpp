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
#include "./config.hpp"
#include <Arduino.h>
#include <FastLED.h>
#include <rvl.hpp>

namespace Lights {

CRGB leds[LED_NUM_PIXELS];

void init() {
  // Segment ends are inclusive and index leds directly, so an end past the last
  // pixel writes into whatever global the linker placed after the array
  for (auto& segment : segments) {
    if (segment.end >= LED_NUM_PIXELS) {
      rvl::error("Segment end %d is past the last pixel %d, clamping",
          segment.end, LED_NUM_PIXELS - 1);
      segment.end = LED_NUM_PIXELS - 1;
    }
  }
  FastLED.addLeds<WS2812B, LED_DATA_PIN, LED_COLOR_MODE>(leds, LED_NUM_PIXELS);
  rvl::info("Lights initialized");
}

uint8_t calculatePixelValue(
    RVLColorComponent* component, uint32_t t, uint8_t x) {
  return sin8(component->w_t * t / 100 + component->w_x * x + component->phi) *
          component->a / 255 +
      component->b;
}

void renderParametric() {
  RVLParametricSettings settings;
  rvl::lockState();
  memcpy(
      &settings, rvl::getParametricSettings(), sizeof(RVLParametricSettings));
  rvl::freeState();
  auto animationClock = rvl::getAnimationClock();

  uint32_t t = animationClock % (settings.timePeriod * 100) * 255 /
      settings.timePeriod;
  for (const auto& segment : segments) {
    for (uint16_t i = segment.start; i <= segment.end; i++) {
      uint16_t normalizedIndex = 0;
      if (segment.reverse) {
        normalizedIndex = (segment.end - i) + segment.offset;
      } else {
        normalizedIndex = (i - segment.start) + segment.offset;
      }
      uint8_t x = 255 * (normalizedIndex % settings.distancePeriod) /
          settings.distancePeriod;

      CHSV layerHSV[NUM_LAYERS];
      CRGB layerRGB[NUM_LAYERS];
      uint8_t alphaValues[NUM_LAYERS];

      for (uint8_t j = 0; j < NUM_LAYERS; j++) {
        layerHSV[j].h = calculatePixelValue(&(settings.layers[j].h), t, x);
        layerHSV[j].s = calculatePixelValue(&(settings.layers[j].s), t, x);
        layerHSV[j].v = calculatePixelValue(&(settings.layers[j].v), t, x);
        alphaValues[j] = calculatePixelValue(&(settings.layers[j].a), t, x);
        hsv2rgb_spectrum(layerHSV[j], layerRGB[j]);
      }
      leds[i] = layerRGB[NUM_LAYERS - 1];
      for (int8_t j = NUM_LAYERS - 2; j >= 0; j--) {
        leds[i] = blend(leds[i], layerRGB[j], alphaValues[j]);
      }
    }
  }
}

void loop() {
  // Blanks rather than showing the local preset or another channel's animation
  // before this node knows what the fleet is showing
  if (rvl::getRenderState() == rvl::RenderState::Unknown) {
    FastLED.clear(true);
    return;
  }

  switch (rvl::getAnimationType()) {
  case rvl::AnimationType::Off:
    // Clears leds[] too, so nothing that shows it later can bring back the
    // frame from before the strip went dark
    FastLED.clear(true);
    return;
  case rvl::AnimationType::Parametric:
    renderParametric();
    break;
  }
  FastLED.setBrightness(rvl::getBrightness());
  FastLED.show();
}

} // namespace Lights
