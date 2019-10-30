/*
Copyright (c) 2018 Bryan Hughes <bryan@nebri.us>

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
#include <rvl-arduino.h>
#include "./controls/controls.h"
#include "./state.h"

namespace Controls {

#define NETWORK_FLASH_RATE 500

bool connectedLEDState = false;
uint32_t nextConnectedLEDFlashTime = 0;

void init() {
  pinMode(CONTROL_DIGIT_1, INPUT);
  pinMode(CONTROL_DIGIT_2, INPUT);
  pinMode(CONTROL_DIGIT_3, INPUT);
  pinMode(CONTROL_DIGIT_4, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  State::getLogger()->info("Controls initialized");
}

void loop() {
  if (State::isWifiConnected()) {
    digitalWrite(GREEN_LED, HIGH);
  } else {
    uint32_t now = millis();
    if (now >= nextConnectedLEDFlashTime) {
      if (connectedLEDState) {
        digitalWrite(GREEN_LED, HIGH);
      } else {
        digitalWrite(GREEN_LED, LOW);
      }
      nextConnectedLEDFlashTime = now + NETWORK_FLASH_RATE;
      connectedLEDState = !connectedLEDState;
    }
  }

  int digit2 = digitalRead(CONTROL_DIGIT_2);
  int digit3 = digitalRead(CONTROL_DIGIT_3);
  int digit4 = digitalRead(CONTROL_DIGIT_4);
  int channel = digit2 * 4 + digit3 * 2 + digit4;
  if (RVLGetChannel() != channel) {
    State::getLogger()->info("Channel changed to %d", channel);
    RVLSetChannel(channel);
  }
}

}  // namespace Controls
