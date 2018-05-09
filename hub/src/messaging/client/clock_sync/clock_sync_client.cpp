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
#include "./messaging/client/clock_sync/clock_sync_client.h"
#include "./messaging/read.h"
#include "./state.h"

namespace ClockSyncClient {

const uint8 protocolVersion = 1;

void init() {
  // Not implemented yet
}

void loop() {
  // Not implemented yet
}

/*
Signature: 4 - "RBTS"
Version: 1 - 1
Type: 1 - 1:broadcast, 2:response
Sequence: 2 - always incrementing
Clock: 4 - direct output of millis()
ClientID: 2 - matches ClientID in GigglePixel
*/

bool parsePacket() {
  Serial.println("Parsing Clock Sync packet");
  uint8 version = Read::read8();
  if (protocolVersion != version) {
    return false;
  }
  uint8 type = Read::read8();
  uint16 seq = Read::read16();
  uint32 commandTime = Read::read32();
  uint16 clientId = Read::read16();

  State::setClockOffset(static_cast<int32>(commandTime) - static_cast<int32>(millis()));

  return true;
}

}  // namespace ClockSyncClient
