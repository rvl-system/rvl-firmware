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
#include "./messaging/stack/protocols/clock_sync/clock_sync.h"
#include "./messaging/stack/transport.h"
#include "./state.h"
#include "./util/logging.h"

namespace ClockSync {

const uint8 protocolVersion = 1;

TransportInterface* transport;

void init(TransportInterface* newTransport) {
  transport = newTransport;
}

void loop() {
  // Not implemented yet
}

/*
Signature: 4 bytes = "CLKS"
Version: 1 byte = 1
Type: 1 byte = 1:reference, 2:response
Sequence: 2 bytes = always incrementing
Clock: 4 bytes = running clock, relative to app start
ClientID: 2 bytes = matches ClientID in GigglePixel, or 0 for transmitter
*/

bool parsePacket() {
  Logging::debug("Parsing Clock Sync packet");
  uint8 version = transport->read8();
  if (protocolVersion != version) {
    return false;
  }
  transport->read8();  // type
  transport->read16();  // seq
  uint32 commandTime = transport->read32();
  transport->read16();  // clientId

  State::setClockOffset(static_cast<int32>(commandTime) - static_cast<int32>(millis()));

  return true;
}

}  // namespace ClockSync
