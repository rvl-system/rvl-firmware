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
#ifdef ESP32
#include <rvl-esp32-wifi.hpp>
#else
#include <rvl-wifi.hpp>
#endif
#include <rvl.hpp>
#include <rvl/config.hpp>

#ifdef HAS_UI
#include "./ui/screen.hpp"
#include "./ui/ui.hpp"
#endif
#ifdef HAS_CONTROLS
#include "./controls/controls.hpp"
#endif
#include "./config.hpp"
#include "./lights.hpp"
#include "./settings.hpp"
#include "./timing_stats.hpp"

TimingStats backgroundStats;
TimingStats foregroundStats;

#ifdef ESP32
RVLESP32Wifi::System* wifiSystem;
#else
RVLWifi::System* wifiSystem;
#endif

void startBackgroundLoop();

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

#ifdef ESP32
  wifiSystem = new RVLESP32Wifi::System(
      Settings::getWiFiSSID(), Settings::getWiFiPassphrase());
#else
  wifiSystem = new RVLWifi::System(
      Settings::getWiFiSSID(), Settings::getWiFiPassphrase());
#endif
  rvl::init(wifiSystem);

  rvl::info("Initializing");
#ifdef ESP32
  rvl::info("Network transport: AsyncUDP (RVLESP32Wifi)");
#else
  rvl::info("Network transport: polling WiFiUDP (RVLWifi)");
#endif

  rvl::info("Device mode: %d", static_cast<int>(rvl::getDeviceMode()));
  rvl::info("Channel: %d", rvl::getChannel());
  rvl::info("Brightness: %d", rvl::getBrightness());

#ifdef HAS_UI
  UI::init();
  Screen::init();
#endif
#ifdef HAS_CONTROLS
  Controls::init();
#endif
  Lights::init();
  startBackgroundLoop();
  rvl::info("Running");
}

// Pacing is the caller's job: the background task paces itself in
// backgroundLoopRunner, while on single-loop platforms the foreground's
// frame-aligned sleep paces both loops together
void backgroundLoop() {
  uint32_t startTime = millis();
#ifdef HAS_UI
  UI::loop();
#endif
#ifdef HAS_CONTROLS
  Controls::loop();
#endif
  Settings::loop();
  rvl::loop();
  backgroundStats.record(millis() - startTime);
  backgroundStats.log("Background loop");
}

// Sleeps to the next frame boundary in animation-clock time, so every node
// renders the same instants, plus a millisecond: delay(n) can return a tick
// short of n ms, and a wake before the boundary would read the old frame
void delayUntilNextFrame() {
  uint32_t clock = rvl::getAnimationClock();
  delay(FRAME_PERIOD - (clock % FRAME_PERIOD) + 1);
}

void backgroundLoopRunner(void* parameters) {
  while (true) {
    backgroundLoop();
    delayUntilNextFrame();
  }
}

// Must run exactly once: the network transport and protocol state driven by the
// background loop are not safe to use from more than one task
void startBackgroundLoop() {
#ifdef ESP32
  // Priority 1: this task polls and sleeps, and must never outrank the WiFi
  // stack (the old 255 was silently clamped to max, starving the WiFi task on
  // this core and delaying packet delivery)
  xTaskCreatePinnedToCore(backgroundLoopRunner, "backgroundLoopRunner", 20192,
      NULL, 1, NULL, xPortGetCoreID() == 0 ? 1 : 0);
#endif
}

// The strip first, so a dissolve's frame goes out right after waking on its
// boundary, and the screen's draw after it
void foregroundLoop() {
  uint32_t startTime = millis();
  Lights::loop();
#ifdef HAS_UI
  Screen::loop();
#endif
  foregroundStats.record(millis() - startTime);
  foregroundStats.log("Foreground loop");
  delayUntilNextFrame();
}

void loop() {
#ifndef ESP32
  backgroundLoop();
#endif
  foregroundLoop();
}