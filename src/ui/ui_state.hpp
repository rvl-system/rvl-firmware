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

#ifndef UI_UI_STATE_H_
#define UI_UI_STATE_H_

#include "./config.hpp"
#include "./ui/control.hpp"
#include <Arduino.h>
#include <vector>

namespace UIState {

extern uint8_t currentTab1Control;
extern uint8_t currentTab2Control;
extern std::vector<Control::Control*> tab1Controls;
extern std::vector<Control::Control*> tab2Controls;

extern uint8_t preset;
extern uint8_t** presetValues;

extern uint8_t currentTab;

extern uint32_t screenTimeout;

void init();

void nextControl();
void previousControl();
void controlIncrease();
void controlDecrease();
void nextTab();

bool isCurrentControlRange();

void resetScreenTimeout();
bool isScreenActive();

} // namespace UIState

#endif // UI_UI_STATE_H_
