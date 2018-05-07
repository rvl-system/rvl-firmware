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
#include "./messaging/client/giggle_pixel/giggle_pixel_client.h"
#include "./messaging/client/giggle_pixel/clock_sync_client.h"
#include "./messaging/client/giggle_pixel/raver_lights_client.h"
#include "./messaging/read.h"
#include "../../../config.h"  // Why does this one single file require ".." but none of the others do?
#include "./state.h"

namespace GigglePixelClient {

void init() {
  ClockSyncClient::init();
  RaverLightsClient::init();
}

void loop() {
  ClockSyncClient::loop();
  RaverLightsClient::loop();
}

void parsePacket() {
  uint32 commandTime;
  Read::read(static_cast<uint8*>(static_cast<void*>(&commandTime)), 4);
  uint8 brightness = Read::read();
  uint8 preset = Read::read();
  uint8 presetValues[NUM_PRESET_VALUES];
  Read::read(presetValues, NUM_PRESET_VALUES);
  State::setAnimation(commandTime, preset, presetValues);
}

}  // namespace GigglePixelClient
