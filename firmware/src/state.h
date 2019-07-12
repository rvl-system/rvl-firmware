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

#include <Arduino.h>
#include <rvl-esp.h>
#include "./codes.h"
#include "./config.h"

namespace State {

void init();
void loop();

uint8_t getBrightness();
void setBrightness(uint8_t brightness);

bool getPowerState();
void setPowerState(bool powerState);

bool isWifiConnected();
void setWifiConnectedState(bool connected);

RVLLogging* getLogger();
void setLogger(RVLLogging* logger);

RVLWaveSettings* getWaveSettings();

}  // namespace State

#endif  // STATE_H_
