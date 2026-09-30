/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

This file is part of RVL Firmware.

RVL Firmware is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RVL Firmware is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RVL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAS_UI

#include "./ui/input.hpp"
#include "../config.hpp"
#include "./ui/ui_state.hpp"
#include <Arduino.h>

namespace Input {

enum class ButtonChangeState : uint8_t { None, Pressed, Holding };

struct ButtonInfo {
  uint32_t holdStartTime;
  byte state;
  byte gpio;
  byte on;
  byte off;
};

ButtonInfo nextControlButtonInfo = {
    UINT32_MAX, BUTTON_UP_OFF, BUTTON_UP, BUTTON_UP_ON, BUTTON_UP_OFF};
ButtonInfo previousControlButtonInfo = {
    UINT32_MAX, BUTTON_DOWN_OFF, BUTTON_DOWN, BUTTON_DOWN_ON, BUTTON_DOWN_OFF};
ButtonInfo increaseValueButtonInfo = {UINT32_MAX, BUTTON_RIGHT_OFF,
    BUTTON_RIGHT, BUTTON_RIGHT_ON, BUTTON_RIGHT_OFF};
ButtonInfo decreaseValueButtonInfo = {
    UINT32_MAX, BUTTON_LEFT_OFF, BUTTON_LEFT, BUTTON_LEFT_ON, BUTTON_LEFT_OFF};
ButtonInfo switchTabButtonInfo = {UINT32_MAX, BUTTON_PRESS_OFF, BUTTON_PRESS,
    BUTTON_PRESS_ON, BUTTON_PRESS_OFF};

void init() {
  pinMode(nextControlButtonInfo.gpio, INPUT);
  pinMode(previousControlButtonInfo.gpio, INPUT);
  pinMode(increaseValueButtonInfo.gpio, INPUT);
  pinMode(decreaseValueButtonInfo.gpio, INPUT);
  pinMode(switchTabButtonInfo.gpio, INPUT);
}

ButtonChangeState getButtonChangeState(ButtonInfo* buttonInfo) {
  ButtonChangeState returnValue = ButtonChangeState::None;
  byte state = digitalRead(buttonInfo->gpio);
  if (state == buttonInfo->on) {
    uint32_t now = millis();
    if (buttonInfo->holdStartTime == UINT32_MAX) {
      buttonInfo->holdStartTime = now;
    }
    uint32_t holdTime = now - buttonInfo->holdStartTime;
    if (buttonInfo->state == buttonInfo->off) {
      if (holdTime > BUTTON_PRESS_ENGAGE_TIME) {
        buttonInfo->state = buttonInfo->on;
        returnValue = ButtonChangeState::Pressed;
      }
    } else if (holdTime > BUTTON_HOLD_ENGAGE_TIME) {
      returnValue = ButtonChangeState::Holding;
    }
  } else {
    buttonInfo->state = buttonInfo->off;
    buttonInfo->holdStartTime = UINT32_MAX;
  }
  return returnValue;
}

void loop() {
  switch (getButtonChangeState(&nextControlButtonInfo)) {
  case ButtonChangeState::Pressed:
    if (UIState::isScreenActive()) {
      UIState::nextControl();
    }
    UIState::resetScreenTimeout();
    break;
  case ButtonChangeState::Holding:
    // Do Nothing
    break;
  case ButtonChangeState::None:
    // Do Nothing
    break;
  }

  switch (getButtonChangeState(&previousControlButtonInfo)) {
  case ButtonChangeState::Pressed:
    if (UIState::isScreenActive()) {
      UIState::previousControl();
    }
    UIState::resetScreenTimeout();
    break;
  case ButtonChangeState::Holding:
    // Do Nothing
    break;
  case ButtonChangeState::None:
    // Do Nothing
    break;
  }

  switch (getButtonChangeState(&increaseValueButtonInfo)) {
  case ButtonChangeState::Pressed:
    if (UIState::isScreenActive()) {
      UIState::controlIncrease();
    }
    UIState::resetScreenTimeout();
    break;
  case ButtonChangeState::Holding:
    if (UIState::isScreenActive() && UIState::isCurrentControlRange()) {
      UIState::controlIncrease();
    }
    UIState::resetScreenTimeout();
    break;
  case ButtonChangeState::None:
    // Do Nothing
    break;
  }

  switch (getButtonChangeState(&decreaseValueButtonInfo)) {
  case ButtonChangeState::Pressed:
    if (UIState::isScreenActive()) {
      UIState::controlDecrease();
    }
    UIState::resetScreenTimeout();
    break;
  case ButtonChangeState::Holding:
    if (UIState::isScreenActive() && UIState::isCurrentControlRange()) {
      UIState::controlDecrease();
    }
    UIState::resetScreenTimeout();
    break;
  case ButtonChangeState::None:
    // Do Nothing
    break;
  }

  switch (getButtonChangeState(&switchTabButtonInfo)) {
  case ButtonChangeState::Pressed:
    if (UIState::isScreenActive()) {
      UIState::nextTab();
    }
    UIState::resetScreenTimeout();
    break;
  case ButtonChangeState::Holding:
    // Do Nothing
    break;
  case ButtonChangeState::None:
    // Do Nothing
    break;
  }
}

} // namespace Input

#endif // HAS_UI
