/*
Copyright (c) 2017 Bryan Hughes <bryan@nebri.us>

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

// TODO(nebrius):
// 1. Remove old animation Files
// 2. Replace with unified wave engine
// 3. Add section to State that converts presets to wave when set

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_INTERRUPT_RETRY_COUNT 0

#include <ESP8266WiFi.h>
#include <FastLED.h>

#define NUM_LEDS 17
#define DATA_PIN 2
#define NUM_WAVES 3

CRGB leds[NUM_LEDS];

struct Wave {
  uint8 a;
  uint8 b;
  uint8 w_t;
  uint8 w_x;
  uint8 phi;
};

struct Waves {
  Wave h;
  Wave s;
  Wave v;
  Wave a;
};

Waves waves[NUM_WAVES];

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(96);

  // Wave 1 init

  waves[0].h.a = 0;
  waves[0].h.b = 180;
  waves[0].h.w_t = 8;
  waves[0].h.w_x = 2;
  waves[0].h.phi = 0;

  waves[0].s.a = 0;
  waves[0].s.b = 255;
  waves[0].s.w_t = 8;
  waves[0].s.w_x = 2;
  waves[0].s.phi = 0;

  waves[0].v.a = 0;
  waves[0].v.b = 255;
  waves[0].v.w_t = 8;
  waves[0].v.w_x = 2;
  waves[0].v.phi = 0;

  waves[0].a.a = 255;
  waves[0].a.b = 0;
  waves[0].a.w_t = 8;
  waves[0].a.w_x = 2;
  waves[0].a.phi = 0;

  // Wave 2 init

  waves[1].h.a = 0;
  waves[1].h.b = 0;
  waves[1].h.w_t = 8;
  waves[1].h.w_x = 0;
  waves[1].h.phi = 0;

  waves[1].s.a = 0;
  waves[1].s.b = 255;
  waves[1].s.w_t = 8;
  waves[1].s.w_x = 0;
  waves[1].s.phi = 0;

  waves[1].v.a = 0;
  waves[1].v.b = 255;
  waves[1].v.w_t = 8;
  waves[1].v.w_x = 0;
  waves[1].v.phi = 0;

  waves[1].a.a = 255;
  waves[1].a.b = 0;
  waves[1].a.w_t = 8;
  waves[1].a.w_x = 0;
  waves[1].a.phi = 0;

  // Wave 3 init

  waves[2].h.a = 0;
  waves[2].h.b = 80;
  waves[2].h.w_t = 0;
  waves[2].h.w_x = 0;
  waves[2].h.phi = 0;

  waves[2].s.a = 0;
  waves[2].s.b = 255;
  waves[2].s.w_t = 0;
  waves[2].s.w_x = 0;
  waves[2].s.phi = 0;

  waves[2].v.a = 0;
  waves[2].v.b = 255;
  waves[2].v.w_t = 0;
  waves[2].v.w_x = 0;
  waves[2].v.phi = 0;

  waves[2].a.a = 0;
  waves[2].a.b = 255;
  waves[2].a.w_t = 0;
  waves[2].a.w_x = 0;
  waves[2].a.phi = 0;
}

void loop() {
  uint32 t = millis() % 25500;
  Serial.println("Loop");
  Serial.println(t);
  for (uint16 i = 0; i < NUM_LEDS; i++) {
    uint8 x = 255 * i / NUM_LEDS;

    CHSV waveHSV[NUM_WAVES];
    CRGB waveRGB[NUM_WAVES];
    uint8 alphaValues[NUM_WAVES];

    for (uint8 j = 0; j < NUM_WAVES; j++) {
      waveHSV[j].h = sin8(waves[j].h.w_t * t / 100 + waves[j].h.w_x * x + waves[j].h.phi) * waves[j].h.a / 255 + waves[j].h.b;
      waveHSV[j].s = sin8(waves[j].s.w_t * t / 100 + waves[j].s.w_x * x + waves[j].s.phi) * waves[j].s.a / 255 + waves[j].s.b;
      waveHSV[j].v = sin8(waves[j].v.w_t * t / 100 + waves[j].v.w_x * x + waves[j].v.phi) * waves[j].v.a / 255 + waves[j].v.b;
      alphaValues[j]  = sin8(waves[j].a.w_t * t / 100 + waves[j].a.w_x * x + waves[j].a.phi) * waves[j].a.a / 255 + waves[j].a.b;
      hsv2rgb_spectrum(waveHSV[j], waveRGB[j]);
    }
    leds[i] = waveRGB[NUM_WAVES - 1];
    for (int8 j = NUM_WAVES - 2; j >= 0; j--) {
      leds[i] = blend(leds[i], waveRGB[j], alphaValues[j]);
    }
  }
  FastLED.show();
  delay(33);
}
