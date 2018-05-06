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

#ifndef STATE_H_
#define STATE_H_

#include "./codes.h"
#include "./config.h"

namespace State {

struct Settings {
  byte preset = DEFAULT_PRESET;
  uint8 brightness = DEFAULT_BRIGHTNESS;
  byte** presetValues;

  byte mode = 0;
  byte currentControl = 0;
  uint8 numClients = 0;

  uint32 commandTime = 0;
};

void init();

Settings* getSettings();

void nextControl();
void controlUp();
void controlDown();

void setClientsConnected(int numConnectedClients);

}  // namespace State

#endif  // STATE_H_
