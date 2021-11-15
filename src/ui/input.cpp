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

#ifdef HAS_UI

#include "./ui/input.hpp"
#include "../config.hpp"
#include "./codes.hpp"
#include "./ui/ui_state.hpp"
#include <Arduino.h>

namespace Input {

enum ButtonChangeState { None = 0, Pressed = 1, Holding = 2 };

struct ButtonInfo {
  uint32_t holdStartTime;
  byte state;
  byte gpio;
  byte on;
  byte off;
};

ButtonInfo nextControlButtonInfo = {
    5, BUTTON_UP_OFF, BUTTON_UP, BUTTON_UP_ON, BUTTON_UP_OFF};
ButtonInfo previousControlButtonInfo = {
    5, BUTTON_DOWN_OFF, BUTTON_DOWN, BUTTON_DOWN_ON, BUTTON_DOWN_OFF};
ButtonInfo increaseValueButtonInfo = {
    5, BUTTON_RIGHT_OFF, BUTTON_RIGHT, BUTTON_RIGHT_ON, BUTTON_RIGHT_OFF};
ButtonInfo decreaseValueButtonInfo = {
    5, BUTTON_LEFT_OFF, BUTTON_LEFT, BUTTON_LEFT_ON, BUTTON_LEFT_OFF};
ButtonInfo switchTabButtonInfo = {
    5, BUTTON_PRESS_OFF, BUTTON_PRESS, BUTTON_PRESS_ON, BUTTON_PRESS_OFF};

void init() {
  pinMode(nextControlButtonInfo.gpio, INPUT);
  pinMode(previousControlButtonInfo.gpio, INPUT);
  pinMode(increaseValueButtonInfo.gpio, INPUT);
  pinMode(decreaseValueButtonInfo.gpio, INPUT);
  pinMode(switchTabButtonInfo.gpio, INPUT);
}

ButtonChangeState getButtonChangeState(ButtonInfo* buttonInfo) {
  ButtonChangeState returnValue = None;
  byte state = digitalRead(buttonInfo->gpio);
  if (state == buttonInfo->on) {
    uint32_t now = millis();
    if (buttonInfo->holdStartTime == -1) {
      buttonInfo->holdStartTime = now;
    }
    uint32_t holdTime = now - buttonInfo->holdStartTime;
    if (buttonInfo->state == buttonInfo->off) {
      if (holdTime > BUTTON_PRESS_ENGAGE_TIME) {
        buttonInfo->state = buttonInfo->on;
        returnValue = Pressed;
      }
    } else if (holdTime > BUTTON_HOLD_ENGAGE_TIME) {
      returnValue = Holding;
    }
  } else {
    buttonInfo->state = buttonInfo->off;
    buttonInfo->holdStartTime = -1;
  }
  return returnValue;
}

void loop() {
  switch (getButtonChangeState(&nextControlButtonInfo)) {
  case Pressed:
    if (UIState::isScreenActive()) {
      UIState::nextControl();
    }
    UIState::resetScreenTimeout();
    break;
  case Holding:
    // Do Nothing
    break;
  case None:
    // Do Nothing
    break;
  }

  switch (getButtonChangeState(&previousControlButtonInfo)) {
  case Pressed:
    if (UIState::isScreenActive()) {
      UIState::previousControl();
    }
    UIState::resetScreenTimeout();
    break;
  case Holding:
    // Do Nothing
    break;
  case None:
    // Do Nothing
    break;
  }

  switch (getButtonChangeState(&increaseValueButtonInfo)) {
  case Pressed:
    if (UIState::isScreenActive()) {
      UIState::controlIncrease();
    }
    UIState::resetScreenTimeout();
    break;
  case Holding:
    if (UIState::isCurrentControlRange()) {
      UIState::controlIncrease();
    }
    break;
  case None:
    // Do Nothing
    break;
  }

  switch (getButtonChangeState(&decreaseValueButtonInfo)) {
  case Pressed:
    if (UIState::isScreenActive()) {
      UIState::controlDecrease();
    }
    UIState::resetScreenTimeout();
    break;
  case Holding:
    if (UIState::isCurrentControlRange()) {
      UIState::controlDecrease();
    }
    break;
  case None:
    // Do Nothing
    break;
  }

  switch (getButtonChangeState(&switchTabButtonInfo)) {
  case Pressed:
    if (UIState::isScreenActive()) {
      UIState::nextTab();
    }
    UIState::resetScreenTimeout();
    break;
  case Holding:
    // Do Nothing
    break;
  case None:
    // Do Nothing
    break;
  }
}

} // namespace Input

#endif // HAS_UI
