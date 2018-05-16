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
#ifdef HAS_NETWORK
#include "./messaging/messaging.h"
#endif
#ifdef HAS_UI
#include "./screen/screen.h"
#include "./input/input.h"
#endif
#ifdef HAS_LIGHTS
#include "./lights/lights.h"
#endif
#include "./state.h"
#include "./config.h"

void setup() {
  delay(200);
  Serial.begin(921600);
  Serial.println();
  Serial.println("Initializing");
  State::init();
#ifdef HAS_UI
  Input::init();
  Screen::init();
#endif
#ifdef HAS_NETWORK
  Messaging::init();
#endif
#ifdef HAS_LIGHTS
  Lights::init();
#endif
  Serial.println("Running");
}

void loop() {
  uint32 startTime = millis();
  State::loop();
#ifdef HAS_UI
  Input::loop();
  Screen::loop();
#endif
#ifdef HAS_NETWORK
  Messaging::loop();
#endif
#ifdef HAS_LIGHTS
  Lights::loop();
#endif
  delay(UPDATE_RATE - (millis() - startTime));
}
