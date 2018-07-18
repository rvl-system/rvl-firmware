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
#include "./ui/ui.h"
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
  UI::init();
#endif
#ifdef HAS_NETWORK
  Messaging::init();
#endif
#ifdef HAS_LIGHTS
  Lights::init();
#endif
  Serial.println("Running");
}

#define NUM_LOOP_SAMPLES 60
uint8 loopTimes[NUM_LOOP_SAMPLES];
uint8 loopIndex = 0;

void loop() {
  uint32 startTime = millis();
  State::loop();
#ifdef HAS_UI
  UI::loop();
#endif
#ifdef HAS_NETWORK
  Messaging::loop();
#endif
#ifdef HAS_LIGHTS
  Lights::loop();
#endif
  uint32 now = millis();
  loopTimes[loopIndex++] = now - startTime;
  if (loopIndex == NUM_LOOP_SAMPLES) {
    loopIndex = 0;
    uint16 sum = 0;
    for (uint8 i = 0; i < NUM_LOOP_SAMPLES; i++) {
      sum += loopTimes[i];
    }
    Serial.print("Average loop time: ");
    Serial.print(sum / NUM_LOOP_SAMPLES);
    Serial.println("ms");
  }
  if (now - startTime > UPDATE_RATE) {
    Serial.print("Warning: system loop took ");
    Serial.print(now - startTime - UPDATE_RATE);
    Serial.println(" ms longer than the update rate");
    delay(1);
  } else {
    delay(UPDATE_RATE - (millis() - startTime));
  }
}
