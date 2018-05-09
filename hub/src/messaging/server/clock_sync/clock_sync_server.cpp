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

#include "./messaging/server/clock_sync/clock_sync_server.h"
#include "./messaging/server/messaging_server.h"
#include "./messaging/broadcast.h"
#include "../../../config.h"
#include "./state.h"

namespace ClockSyncServer {

const uint8 protocolVersion = 1;

uint32 nextSyncTime = 0;
uint16 seq = 0;

void update();

void init() {
  // Nothing to do
}

void loop() {
  if (millis() < nextSyncTime) {
    return;
  }
  nextSyncTime = millis() + CLOCK_SYNC_INTERVAL;
  update();
}

/*
Signature: 4 - "RBTS"
Version: 1 - 1
Type: 1 - 1:broadcast, 2:response
Sequence: 2 - always incrementing
Clock: 4 - direct output of millis()
ClientID: 2 - matches ClientID in GigglePixel
*/

void update() {
  uint32 clock = State::getSettings()->clock;
  Serial.print("Syncing Clock: ");
  Serial.println(clock);

  Broadcast::begin();
  uint8* signature = const_cast<uint8*>(reinterpret_cast<const uint8*>("RBTS"));
  Broadcast::write(signature, sizeof(uint8) * 4);
  Broadcast::write8(protocolVersion);
  Broadcast::write8(1);  // Type, hardcoded for now
  Broadcast::write16(++seq);
  Broadcast::write32(clock);
  Broadcast::write16(State::getSettings()->id);
  Broadcast::end();
}

}  // namespace ClockSyncServer
