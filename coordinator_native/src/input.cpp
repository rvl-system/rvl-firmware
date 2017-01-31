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

byte nextControlState = LOW;
int nextControlHoldTime = 0;

byte controlUpState = LOW;
int controlUpHoldTime = 0;

byte controlDownState = LOW;
int controlDownHoldTime = 0;

void Input::init() {
  pinMode(BUTTON_NEXT_CONTROL, INPUT);
  pinMode(BUTTON_CONTROL_UP, INPUT);
  pinMode(BUTTON_CONTROL_DOWN, INPUT);
  Serial.println("Input initialized");
}

void Input::loop() {

  State::Settings* settings = State::getSettings();

  int currentNextControlState = digitalRead(BUTTON_NEXT_CONTROL);
  if (currentNextControlState != nextControlState) {
    nextControlState = currentNextControlState;
    if (nextControlState == HIGH) {
      State::setActive();
    } else {
      State::setIdling();
      if (nextControlHoldTime >= CONTROL_PRESS_ENGAGE_TIME) {
        State::nextControl();
      }
      nextControlHoldTime = 0;
    }
  }
  if (nextControlState == HIGH) {
    nextControlHoldTime++;
  }

  int currentControlUpState = digitalRead(BUTTON_CONTROL_UP);
  if (currentControlUpState != controlUpState) {
    controlUpState = currentControlUpState;
    if (controlUpState == HIGH) {
      State::setActive();
    } else {
      State::setIdling();
      if (controlUpHoldTime >= CONTROL_PRESS_ENGAGE_TIME) {
        State::controlUp();
      }
      controlUpHoldTime = 0;
    }
  }
  if (controlUpState == HIGH) {
    controlUpHoldTime++;
    if (controlUpHoldTime >= CONTROL_HOLD_ENGAGE_TIME &&
      controlUpHoldTime % CONTROL_HOLD_ENGAGE_STEP_INTERVAL == 0 &&
      settings->currentControl != Codes::Control::Preset &&
      settings->idleState == Codes::IdleState::Active
    ) {
      State::controlUp();
    }
  }

  int currentControlDownState = digitalRead(BUTTON_CONTROL_DOWN);
  if (currentControlDownState != controlDownState) {
    controlDownState = currentControlDownState;
    if (controlDownState == HIGH) {
      State::setActive();
    } else {
      State::setIdling();
      if (controlDownHoldTime >= CONTROL_PRESS_ENGAGE_TIME) {
        State::controlDown();
      }
      controlDownHoldTime = 0;
    }
  }
  if (controlDownState == HIGH) {
    controlDownHoldTime++;
    if (controlDownHoldTime >= CONTROL_HOLD_ENGAGE_TIME &&
      controlDownHoldTime % CONTROL_HOLD_ENGAGE_STEP_INTERVAL == 0 &&
      settings->currentControl != Codes::Control::Preset &&
      settings->idleState == Codes::IdleState::Active
    ) {
      State::controlDown();
    }
  }
}
