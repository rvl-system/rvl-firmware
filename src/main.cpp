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
#include <rvl.h>
#include <rvl-wifi.h>
#ifdef HAS_UI
#include "./ui/ui.h"
#endif
#ifdef HAS_CONTROLS
#include "./controls/controls.h"
#endif
#ifdef HAS_LIGHTS
#include "./lights.h"
#endif
#include "./settings.h"
#include "./state.h"
#include "./config.h"

RVLWifi::Transport* transport;

void setup() {
  Settings::init();

  Serial.begin(SERIAL_BAUDRATE);
  rvl::setLogLevel(rvl::LogLevel::Debug);

  transport = new RVLWifi::Transport(Settings::getWiFiSSID(), Settings::getWiFiPassphrase(), Settings::getPort());
  rvl::init(transport);

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

bool animationLoopStarted = false;

void loop() {
  State::loop();
#ifdef HAS_UI
  UI::loop();
#endif
#ifdef HAS_CONTROLS
  Controls::loop();
#endif
  rvl::loop();
#ifdef HAS_LIGHTS
  if (!animationLoopStarted) {
    animationLoopStarted = true;
    Lights::startAnimationLoop();
  }
  Lights::loop();
#endif
}
