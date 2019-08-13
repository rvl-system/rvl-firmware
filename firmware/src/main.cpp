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

#include <Arduino.h>
#include <rvl-esp.h>
#ifdef HAS_UI
#include "./ui/ui.h"
#endif
#ifdef HAS_CONTROLS
#include "./controls/controls.h"
#endif
#ifdef HAS_LIGHTS
#include "./lights.h"
#endif
#ifdef HAS_CLOCK
#include "./clock.h"
#endif
#ifdef HAS_EXTERNAL_SIGN
#include "./external_sign.h"
#endif
#include "./state.h"
#include "./config.h"

void setup() {
  delay(200);

  Serial.begin(SERIAL_BAUDRATE);
  State::setLogger(RVLESPInitLogging(RVLLogLevel::Debug));
  RVLESPInitNetwork(WIFI_SSID, WIFI_PASSPHRASE, SERVER_PORT);

  State::getLogger()->info("Initializing");
  State::init();
#ifdef HAS_UI
  UI::init();
#endif
#ifdef HAS_CONTROLS
  Controls::init();
#endif
#ifdef HAS_LIGHTS
  Lights::init();
#endif
#ifdef HAS_CLOCK
  Clock::init();
#endif
#ifdef HAS_EXTERNAL_SIGN
  ExternalSign::init();
#endif

  // Create a default animation
  RVLWaveSettings newSettings;

  // Wave wave
  newSettings.waves[0].h.b = 0;
  newSettings.waves[0].s.b = 255;
  newSettings.waves[0].v.b = 255;
  newSettings.waves[0].a.a = 255;
  newSettings.waves[0].a.w_t = 8;
  newSettings.waves[0].a.w_x = 2;

  // Foreground wave
  newSettings.waves[1].h.b = 170;
  newSettings.waves[1].s.b = 255;
  newSettings.waves[1].v.b = 255;
  newSettings.waves[1].a.w_t = 8;
  newSettings.waves[1].a.a = 255;

  // Background wave
  newSettings.waves[2].h.b = 85;
  newSettings.waves[2].s.b = 255;
  newSettings.waves[2].v.b = 255;
  newSettings.waves[2].a.a = 255;

  RVLESPSetWaveSettings(&newSettings);

  State::getLogger()->info("Running");
}

#define NUM_LOOP_SAMPLES 60
uint8_t loopTimes[NUM_LOOP_SAMPLES];
uint8_t loopIndex = 0;

void loop() {
  uint32_t startTime = millis();
  State::loop();
#ifdef HAS_UI
  UI::loop();
#endif
#ifdef HAS_CONTROLS
  Controls::loop();
#endif
  RVLESPLoop();
#ifdef HAS_LIGHTS
  Lights::loop();
#endif
#ifdef HAS_CLOCK
  Clock::loop();
#endif
#ifdef HAS_EXTERNAL_SIGN
  ExternalSign::loop();
#endif
  uint32_t now = millis();
  loopTimes[loopIndex++] = now - startTime;
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
    State::getLogger()->debug("Performance stats: Avg=%d Min=%d Max=%d", sum / NUM_LOOP_SAMPLES, min, max);
  }
  if (now - startTime > UPDATE_RATE) {
    State::getLogger()->info("Warning: system loop took %dms longer than the update rate",
      now - startTime - UPDATE_RATE);
    delay(1);
  } else {
    delay(UPDATE_RATE - (millis() - startTime));
  }
}
