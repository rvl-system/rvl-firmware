/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

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

#include <Arduino.h>
#include <rvl-wifi.hpp>
#include <rvl.hpp>

#ifdef HAS_UI
#include "./ui/screen.hpp"
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
uint8_t backgroundLoopTimes[NUM_LOOP_SAMPLES];
uint8_t backgroundLoopIndex = 0;
uint8_t foregroundLoopTimes[NUM_LOOP_SAMPLES];
uint8_t foregroundLoopIndex = 0;

RVLWifi::System* wifiSystem;

void setup() {
  Settings::init();

  Serial.begin(SERIAL_BAUDRATE);
#ifdef LOG_DEBUG_ENABLED
  rvl::setLogLevel(rvl::LogLevel::Debug);
#elif LOG_INFO_ENABLED
  rvl::setLogLevel(rvl::LogLevel::Info);
#else
  rvl::setLogLevel(rvl::LogLevel::Error);
#endif

  wifiSystem = new RVLWifi::System(Settings::getWiFiSSID(),
      Settings::getWiFiPassphrase(), Settings::getPort());
  rvl::init(wifiSystem);

  rvl::info("Initializing");

  rvl::info("Device mode: %d", rvl::getDeviceMode());
  rvl::info("Channel: %d", rvl::getChannel());
  rvl::info("Brightness: %d", rvl::getBrightness());

  State::init();
#ifdef HAS_UI
  UI::init();
  Screen::init();
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

void backgroundLoop() {
  uint32_t startTime = millis();
  State::loop();
#ifdef HAS_UI
  UI::loop();
#endif
#ifdef HAS_CONTROLS
  Controls::loop();
#endif
  rvl::loop();
  uint32_t now = millis();
  if (backgroundLoopIndex < NUM_LOOP_SAMPLES) {
    backgroundLoopTimes[backgroundLoopIndex++] = now - startTime;
  }
  if (now - startTime > UPDATE_RATE) {
    delay(1);
  } else {
    delay(UPDATE_RATE - (millis() - startTime));
  }
  if (backgroundLoopIndex == NUM_LOOP_SAMPLES) {
    backgroundLoopIndex = 0;
    uint16_t sum = 0;
    uint8_t min = 255;
    uint8_t max = 0;
    for (uint8_t i = 0; i < NUM_LOOP_SAMPLES; i++) {
      sum += backgroundLoopTimes[i];
      if (backgroundLoopTimes[i] < min) {
        min = backgroundLoopTimes[i];
      }
      if (backgroundLoopTimes[i] > max) {
        max = backgroundLoopTimes[i];
      }
    }
    rvl::debug("Background loop stats: Avg=%d Min=%d Max=%d",
        sum / NUM_LOOP_SAMPLES, min, max);
  }
}

void backgroundLoopRunner(void* parameters) {
  while (true) {
    backgroundLoop();
  }
}

void startBackgroundLoop() {
#ifdef ESP32
  xTaskCreatePinnedToCore(backgroundLoopRunner, "backgroundLoopRunner", 20192,
      NULL, 255, NULL, xPortGetCoreID() ? 0 : 1);
#endif
}

void foregroundLoop() {
  uint32_t startTime = millis();
#ifdef HAS_UI
  Screen::loop();
#endif
#ifdef HAS_LIGHTS
  Lights::loop();
#endif
  uint32_t now = millis();
  if (foregroundLoopIndex < NUM_LOOP_SAMPLES) {
    foregroundLoopTimes[foregroundLoopIndex++] = now - startTime;
  }
  if (now - startTime > UPDATE_RATE) {
    delay(1);
  } else {
    delay(UPDATE_RATE - (millis() - startTime));
  }
  if (foregroundLoopIndex == NUM_LOOP_SAMPLES) {
    foregroundLoopIndex = 0;
    uint16_t sum = 0;
    uint8_t min = 255;
    uint8_t max = 0;
    for (uint8_t i = 0; i < NUM_LOOP_SAMPLES; i++) {
      sum += foregroundLoopTimes[i];
      if (foregroundLoopTimes[i] < min) {
        min = foregroundLoopTimes[i];
      }
      if (foregroundLoopTimes[i] > max) {
        max = foregroundLoopTimes[i];
      }
    }
    rvl::debug("Foreground loop stats: Avg=%d Min=%d Max=%d",
        sum / NUM_LOOP_SAMPLES, min, max);
  }
}

bool backgroundLoopStarted = false;
void loop() {
#ifdef ESP32
  if (!backgroundLoopStarted) {
    backgroundLoopStarted = true;
    startBackgroundLoop();
  }
#else
  backgroundLoop();
#endif

  foregroundLoop();
}