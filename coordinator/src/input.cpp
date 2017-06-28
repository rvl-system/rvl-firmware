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
#include "input.h"
#include "state.h"
#include "common/codes.h"
#include "config.h"

byte nextControlState = BUTTON_NEXT_CONTROL_OFF;
int nextControlHoldTime = 0;

void Input::init() {
  pinMode(BUTTON_NEXT_CONTROL, INPUT_PULLUP);
  Serial.println("Input initialized");
}

void Input::loop() {

  State::Settings* settings = State::getSettings();

  int newNextControlState = digitalRead(BUTTON_NEXT_CONTROL);
  if (newNextControlState != nextControlState) {
    nextControlState = newNextControlState;
    if (nextControlState == BUTTON_NEXT_CONTROL_OFF) {
      if (State::getSettings()->idleState != Codes::IdleState::DeepIdle &&
        nextControlHoldTime >= CONTROL_PRESS_ENGAGE_TIME
      ) {
        State::nextControl();
      }
      State::setActive();
      nextControlHoldTime = 0;
      State::setIdling();
    }
  }
  if (nextControlState == BUTTON_NEXT_CONTROL_ON) {
    nextControlHoldTime++;
  }

  // if (newEncoderPosition >= encoderPosition + ENCODER_STEPS_PER_CLICK ||
  //   newEncoderPosition <= encoderPosition - ENCODER_STEPS_PER_CLICK
  // ) {
  //   if (State::getSettings()->idleState != Codes::IdleState::DeepIdle) {
  //     if (newEncoderPosition > encoderPosition) {
  //       State::controlUp();
  //     } else {
  //       State::controlDown();
  //     }
  //   }
  //   State::setActive();
  //   encoderPosition = newEncoderPosition;
  //   State::setIdling();
  // }
}
