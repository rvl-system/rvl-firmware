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
#include <rvl-wifi.hpp>
#include <rvl.hpp>

#ifdef HAS_UI
#include "./ui/ui.hpp"
#endif
#ifdef HAS_CONTROLS
#include "./controls/controls.hpp"
#endif
#ifdef HAS_LIGHTS
#include "./lights.hpp"
#endif
#include "./config.hpp"
#include "./settings.hpp"
#include "./state.hpp"

#define NUM_LOOP_SAMPLES 60
uint8_t loopTimes[NUM_LOOP_SAMPLES];
uint8_t loopIndex = 0;

RVLWifi::System* wifiSystem;

void setup() {
  Settings::init();

  Serial.begin(SERIAL_BAUDRATE);
  rvl::setLogLevel(rvl::LogLevel::Debug);

  wifiSystem = new RVLWifi::System(Settings::getWiFiSSID(),
      Settings::getWiFiPassphrase(), Settings::getPort());
  rvl::init(wifiSystem);

  rvl::info("Initializing");

  State::init();
#ifdef HAS_UI
  UI::init();
#else
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

  rvl::setWaveSettings(&newSettings);
#endif
#ifdef HAS_CONTROLS
  Controls::init();
#endif
#ifdef HAS_LIGHTS
  Lights::init();
#endif
  rvl::info("Running");
}

void loop() {
  uint32_t startTime = millis();
  State::loop();
#ifdef HAS_UI
  UI::loop();
#endif
#ifdef HAS_CONTROLS
  Controls::loop();
#endif
  rvl::loop();
#ifdef HAS_LIGHTS
  Lights::loop();
#endif
  uint32_t now = millis();
  if (loopIndex < NUM_LOOP_SAMPLES) {
    loopTimes[loopIndex++] = now - startTime;
  }
  if (now - startTime > UPDATE_RATE) {
    delay(1);
  } else {
    delay(UPDATE_RATE - (millis() - startTime));
  }
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
    rvl::info("Main loop stats: Avg=%d Min=%d Max=%d", sum / NUM_LOOP_SAMPLES,
        min, max);
  }
}
