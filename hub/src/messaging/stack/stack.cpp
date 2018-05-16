/*
Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>

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
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "./messaging/stack/stack.h"
#include "./messaging/stack/transport.h"
#include "./messaging/stack/protocols/clock_sync/clock_sync.h"
#include "./messaging/stack/protocols/giggle_pixel/giggle_pixel.h"
#include "../../config.h"  // Why does this one single file require ".." but none of the others do?
#include "./codes.h"
#include "./state.h"

namespace Stack {

TransportInterface* transport;

void init(TransportInterface* newTransport) {
  transport = newTransport;
  ClockSync::init(newTransport);
  GigglePixel::init(newTransport);
}

void parsePacket() {
  uint8 signature[4];
  transport->read(signature, 4);
  if (
    signature[0] == ClockSync::signature[0] &&
    signature[1] == ClockSync::signature[1] &&
    signature[2] == ClockSync::signature[2] &&
    signature[3] == ClockSync::signature[3]
  ) {
    ClockSync::parsePacket();
  } else if (
    signature[0] == GigglePixel::signature[0] &&
    signature[1] == GigglePixel::signature[1] &&
    signature[2] == GigglePixel::signature[2] &&
    signature[3] == GigglePixel::signature[3]
  ) {
    GigglePixel::parsePacket();
  }

  ClockSync::loop();
  GigglePixel::loop();
}

}  // namespace Stack
