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

namespace UIState {

extern uint8 currentControl;

#define NUM_COMMON_CONTROLS 4

const uint8 numControls[] = {
  NUM_COMMON_CONTROLS + 1,  // Rainbow
  NUM_COMMON_CONTROLS + 3,  // Pulse
  NUM_COMMON_CONTROLS + 5,  // Wave
};

void nextControl();
void previousControl();
void controlIncrease();
void controlDecrease();
bool isCurrentControlARange();

}  // namespace UIState

#endif  // UI_UI_STATE_H_
