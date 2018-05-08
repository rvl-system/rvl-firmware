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

#include <Arduino.h>
#include "./messaging/client/giggle_pixel/raver_lights_client.h"
#include "./messaging/read.h"
#include "../../../config.h"  // Why does this one single file require ".." but none of the others do?
#include "./state.h"

namespace RaverLightsClient {

void init() {
  // Not implemented yet
}

void loop() {
  // Not implemented yet
}

void parsePacket() {
  Serial.println("Parsing Raver Lights sub-packet");
  uint32 commandTime;
  Read::read(static_cast<uint8*>(static_cast<void*>(&commandTime)), 4);
  uint8 brightness = Read::read8();
  uint8 preset = Read::read8();
  uint8 presetValues[NUM_PRESET_VALUES];
  Read::read(presetValues, NUM_PRESET_VALUES);
  State::setAnimation(commandTime, preset, presetValues);
}

}  // namespace RaverLightsClient
