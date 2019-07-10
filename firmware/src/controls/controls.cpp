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
#include <rvl-esp.h>
#include "./controls/controls.h"
#include "./state.h"

namespace Controls {

void init() {
  pinMode(CONTROL_DIGIT_1, INPUT);
  pinMode(CONTROL_DIGIT_2, INPUT);
  pinMode(CONTROL_DIGIT_3, INPUT);
  pinMode(CONTROL_DIGIT_4, INPUT);
  State::getLogger()->info("Controls initialized");
}

void loop() {
  int digit1 = digitalRead(CONTROL_DIGIT_1);
  int digit2 = digitalRead(CONTROL_DIGIT_2);
  int digit3 = digitalRead(CONTROL_DIGIT_3);
  int digit4 = digitalRead(CONTROL_DIGIT_4);
  int value = digit1 * 8 + digit2 * 4 + digit3 * 2 + digit4;
  if (RVLESPGetChannel() != value) {
    State::getLogger()->info("Channel changed to %d", value);
    RVLESPSetChannel(value);
  }
}

}  // namespace Controls
