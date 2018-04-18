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
#include "codes.h"
#include "config.h"

namespace Input {

  enum ButtonChangeState {
    None = 0,
    Pressed = 1,
    Holding = 2
  };

  struct ButtonInfo {
    unsigned long holdStartTime;
    byte state;
    byte gpio;
    byte on;
    byte off;
    bool isWakeupPress;
  };

  ButtonInfo nextButtonInfo = { 5, BUTTON_NEXT_OFF, BUTTON_NEXT, BUTTON_NEXT_ON, BUTTON_NEXT_OFF, false };
  ButtonInfo upButtonInfo = { 5, BUTTON_UP_OFF, BUTTON_UP, BUTTON_UP_ON, BUTTON_UP_OFF, false };
  ButtonInfo downButtonInfo = { 5, BUTTON_DOWN_OFF, BUTTON_DOWN, BUTTON_DOWN_ON, BUTTON_DOWN_OFF, false };

  void init() {
    pinMode(nextButtonInfo.gpio, INPUT);
    pinMode(upButtonInfo.gpio, INPUT);
    pinMode(downButtonInfo.gpio, INPUT);
    Serial.println("Input initialized");
  }

  ButtonChangeState getButtonChangeState(ButtonInfo* buttonInfo) {
    ButtonChangeState returnValue = None;
    byte state = digitalRead(buttonInfo->gpio);
    if (state == buttonInfo->on) {
      unsigned long now = millis();
      if (buttonInfo->holdStartTime == -1) {
        buttonInfo->holdStartTime = now;
      }
      unsigned long holdTime = now - buttonInfo->holdStartTime;
      if (buttonInfo->state == buttonInfo->off) {
        buttonInfo->isWakeupPress = State::getSettings()->idleState == Codes::IdleState::DeepIdle;
        if (holdTime > BUTTON_PRESS_ENGAGE_TIME) {
          buttonInfo->state = buttonInfo->on;
          State::setActive();
          if (!buttonInfo->isWakeupPress) {
            returnValue = Pressed;
          }
        }
      } else if (!buttonInfo->isWakeupPress && holdTime > BUTTON_HOLD_ENGAGE_TIME) {
        returnValue = Holding;
      }
    } else {
      if (buttonInfo->state != buttonInfo->off) {
        State::setIdling();
      }
      buttonInfo->state = buttonInfo->off;
      buttonInfo->holdStartTime = -1;
    }
    return returnValue;
  }

  void loop() {

    switch (getButtonChangeState(&nextButtonInfo)) {
      case Pressed:
        State::nextControl();
        break;
      case Holding:
        // Do Nothing
        break;
      case None:
        // Do Nothing
        break;
    }

    switch (getButtonChangeState(&upButtonInfo)) {
      case Pressed:
        State::controlUp();
        break;
      case Holding:
        if (State::getSettings()->currentControl != Codes::Control::Preset) {
          State::controlUp();
        }
        break;
      case None:
        // Do Nothing
        break;
    }

    switch (getButtonChangeState(&downButtonInfo)) {
      case Pressed:
        State::controlDown();
        break;
      case Holding:
        if (State::getSettings()->currentControl != Codes::Control::Preset) {
          State::controlDown();
        }
        break;
      case None:
        // Do Nothing
        break;
    }

  }

}
