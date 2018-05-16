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
#include "./ui/input.h"
#include "./ui/screen.h"
#include "./codes.h"
#include "../config.h"

namespace Input {

enum ButtonChangeState {
  None = 0,
  Pressed = 1,
  Holding = 2
};

struct ButtonInfo {
  uint32 holdStartTime;
  byte state;
  byte gpio;
  byte on;
  byte off;
};

ButtonInfo nextButtonInfo = { 5, BUTTON_NEXT_OFF, BUTTON_NEXT, BUTTON_NEXT_ON, BUTTON_NEXT_OFF };
ButtonInfo upButtonInfo = { 5, BUTTON_UP_OFF, BUTTON_UP, BUTTON_UP_ON, BUTTON_UP_OFF };
ButtonInfo downButtonInfo = { 5, BUTTON_DOWN_OFF, BUTTON_DOWN, BUTTON_DOWN_ON, BUTTON_DOWN_OFF };

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
    uint32 now = millis();
    if (buttonInfo->holdStartTime == -1) {
      buttonInfo->holdStartTime = now;
    }
    uint32 holdTime = now - buttonInfo->holdStartTime;
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
  switch (getButtonChangeState(&nextButtonInfo)) {
    case Pressed:
      Screen::nextControl();
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
      Screen::controlUp();
      break;
    case Holding:
      if (Screen::currentControl != Codes::Control::Preset) {
        Screen::controlUp();
      }
      break;
    case None:
      // Do Nothing
      break;
  }

  switch (getButtonChangeState(&downButtonInfo)) {
    case Pressed:
      Screen::controlDown();
      break;
    case Holding:
      if (Screen::currentControl != Codes::Control::Preset) {
        Screen::controlDown();
      }
      break;
    case None:
      // Do Nothing
      break;
  }
}

}  // namespace Input
