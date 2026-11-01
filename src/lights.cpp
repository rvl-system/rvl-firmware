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
#include <rvl/config.hpp>
#include <variant>

namespace Lights {

CRGB leds[LED_MAX_PIXELS];
// The outgoing scene of a dissolve. Here rather than on the loop task's stack
CRGB fadeFrom[LED_MAX_PIXELS];
bool blanked = false;
CLEDController* controller;
// The configured count, read once per frame; only ever touched from the loop
// task
uint16_t pixelCount = LED_MAX_PIXELS;

void init() {
  // Segment ends are inclusive and index leds directly, so an end past the last
  // pixel writes into whatever global the linker placed after the array
  for (auto& segment : segments) {
    if (segment.end >= LED_MAX_PIXELS) {
      rvl::error("Segment end %d is past the last pixel %d, clamping",
          segment.end, LED_MAX_PIXELS - 1);
      segment.end = LED_MAX_PIXELS - 1;
    }
  }
  // Told RGB: the configured order is applied by permuteColorOrder each frame
  controller =
      &FastLED.addLeds<WS2812B, LED_DATA_PIN, RGB>(leds, LED_MAX_PIXELS);
  rvl::info("Lights initialized");
}

// On this task, where show() runs, so a change can't land mid-transfer
void applyPixelCount() {
  uint16_t newPixelCount = rvl::getPixelCount();
  if (newPixelCount != pixelCount) {
    pixelCount = newPixelCount;
    controller->setLeds(leds, pixelCount);
  }
}

// Which component lands in each wire position, indexed by ColorOrder
const uint8_t COLOR_ORDER_MAP[][3] = {
    {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};

// FastLED sends the three bytes of each pixel in order, so a strip wired
// another way gets them swapped into the order it reads
void permuteColorOrder(CRGB* pixels, uint16_t count, rvl::ColorOrder order) {
  if (order == rvl::ColorOrder::RGB) {
    return;
  }
  const uint8_t* map = COLOR_ORDER_MAP[static_cast<uint8_t>(order)];
  for (uint16_t i = 0; i < count; i++) {
    CRGB p = pixels[i];
    pixels[i] = CRGB(p.raw[map[0]], p.raw[map[1]], p.raw[map[2]]);
  }
}

uint8_t calculatePixelValue(
    const RVLColorComponent& component, uint32_t t, uint8_t x) {
  return sin8(component.w_t * t / 100 + component.w_x * x + component.phi) *
      component.a / 255 +
      component.b;
}

void renderParametric(
    const RVLParametricSettings& settings, uint32_t animationClock, CRGB* out) {
  uint32_t t =
      animationClock % (settings.timePeriod * 100) * 255 / settings.timePeriod;
  for (const auto& segment : segments) {
    // Only the configured pixels: a short strip renders only its own length
    if (segment.start >= pixelCount) {
      continue;
    }
    uint16_t end = min<uint16_t>(segment.end, pixelCount - 1);
    for (uint16_t i = segment.start; i <= end; i++) {
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
        layerHSV[j].h = calculatePixelValue(settings.layers[j].h, t, x);
        layerHSV[j].s = calculatePixelValue(settings.layers[j].s, t, x);
        layerHSV[j].v = calculatePixelValue(settings.layers[j].v, t, x);
        alphaValues[j] = calculatePixelValue(settings.layers[j].a, t, x);
        hsv2rgb_spectrum(layerHSV[j], layerRGB[j]);
      }
      out[i] = layerRGB[NUM_LAYERS - 1];
      for (int8_t j = NUM_LAYERS - 2; j >= 0; j--) {
        out[i] = blend(out[i], layerRGB[j], alphaValues[j]);
      }
    }
  }
}

// At the frame's time rather than a live clock read, so both renders of a
// dissolve, and every board's render of a frame, see the same instant. Cleared
// first, so a pixel outside every segment shows black rather than whatever
// last wrote it, such as identify
void renderScene(const RVLScene& scene, uint32_t frame, CRGB* out) {
  fill_solid(out, pixelCount, CRGB::Black);
  const auto* settings = std::get_if<RVLParametricSettings>(&scene.content);
  if (settings != nullptr) {
    renderParametric(*settings, frame * FRAME_PERIOD, out);
  }
}

void loop() {
  applyPixelCount();
  // Identify overrides everything, blank included, so a board in a dark fleet
  // can still be found
  if (rvl::isIdentifying()) {
    fill_solid(
        leds, pixelCount, millis() / 500 % 2 == 0 ? CRGB::White : CRGB::Black);
    FastLED.setBrightness(rvl::getBrightness());
    FastLED.show();
    blanked = false;
    return;
  }

  rvl::RenderPlan plan = rvl::getRenderPlan();
  // Blanks rather than showing the local preset or another channel's animation
  // before this node knows what the fleet is showing, and once an off scene has
  // finished dissolving in
  bool blank = rvl::getRenderState() == rvl::RenderState::Unknown ||
      (std::holds_alternative<RVLOff>(plan.current.content) && !plan.fading);
  if (blank) {
    if (!blanked) {
      // Clears leds[] too, so nothing that shows it later can bring back the
      // frame from before the strip went dark
      FastLED.clear(true);
      blanked = true;
    }
    return;
  }
  blanked = false;

  renderScene(plan.current, plan.frame, leds);
  if (plan.fading) {
    renderScene(plan.previous, plan.frame, fadeFrom);
    for (uint16_t i = 0; i < pixelCount; i++) {
      leds[i] = blend(fadeFrom[i], leds[i], plan.amount);
    }
  }
  permuteColorOrder(leds, pixelCount, rvl::getColorOrder());
  FastLED.setBrightness(rvl::getBrightness());
  FastLED.show();
}

} // namespace Lights
