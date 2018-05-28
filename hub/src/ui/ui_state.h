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

#ifndef UI_UI_STATE_H_
#define UI_UI_STATE_H_

#include <Arduino.h>
#include <vector>
#include "./ui/controls/control.h"

namespace UIState {

extern uint8 currentControl;
extern std::vector<Control::Control*> controls;

void init();

void nextControl();
void previousControl();
void controlIncrease();
void controlDecrease();

bool isCurrentControlRange();

}  // namespace UIState

#endif  // UI_UI_STATE_H_
