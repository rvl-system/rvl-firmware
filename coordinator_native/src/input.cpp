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
#include <Encoder.h>
#include "input.h"
#include "state.h"
#include "common/codes.h"
#include "config.h"

byte nextControlState = BUTTON_NEXT_CONTROL_OFF;
int nextControlHoldTime = 0;

Encoder encoder(ENCODER_A, ENCODER_B);
long currentEncoderPosition = -999;

void Input::init() {
  pinMode(BUTTON_NEXT_CONTROL, INPUT_PULLUP);
  Serial.println("Input initialized");
}

void Input::loop() {

  State::Settings* settings = State::getSettings();

  int currentNextControlState = digitalRead(BUTTON_NEXT_CONTROL);
  if (currentNextControlState != nextControlState) {
    nextControlState = currentNextControlState;
    if (nextControlState == BUTTON_NEXT_CONTROL_ON) {
      State::setActive();
    } else {
      State::setIdling();
      if (nextControlHoldTime >= CONTROL_PRESS_ENGAGE_TIME) {
        State::nextControl();
      }
      nextControlHoldTime = 0;
    }
  }
  if (nextControlState == BUTTON_NEXT_CONTROL_ON) {
    nextControlHoldTime++;
  }

  long newEncoderPosition = encoder.read();
  if (newEncoderPosition >= currentEncoderPosition + ENCODER_STEPS_PER_CLICK ||
    newEncoderPosition <= currentEncoderPosition - ENCODER_STEPS_PER_CLICK
  ) {
    if (newEncoderPosition > currentEncoderPosition) {
      State::controlUp();
    } else {
      State::controlDown();
    }
    State::setActive();
    currentEncoderPosition = newEncoderPosition;
  }
}
