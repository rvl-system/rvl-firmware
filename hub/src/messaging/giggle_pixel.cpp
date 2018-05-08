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
#include <ESP8266WiFi.h>
#include "./messaging/giggle_pixel.h"
#include "./messaging/broadcast.h"
#include "./messaging/read.h"

namespace GigglePixel {

const uint8 protocolVersion = 2;
uint16 id = 0;

void setClientId(uint16 newId) {
  id = newId;
}

void broadcastHeader(uint8 packetType, uint8 priority, uint16 length) {
  uint8* signature = const_cast<uint8*>(reinterpret_cast<const uint8*>("GLPX"));
  Broadcast::write(signature, sizeof(uint8) * 4);
  Broadcast::write8(protocolVersion);
  Broadcast::write8(length >> 8);
  Broadcast::write8(length & 0xFF);
  Broadcast::write8(packetType);
  Broadcast::write8(priority);
  Broadcast::write8(0); // reserved
  Broadcast::write8(id >> 8);
  Broadcast::write8(id & 0xFF);
}

bool readHeader(GigglePixelHeaderDetails& header) {
  header.protocolVersion = Read::read8();
  if (header.protocolVersion != protocolVersion) {
    return false;
  }
  header.length = Read::read8() << 8 | Read::read8();
  header.packetType = Read::read8();
  header.priority = Read::read8();
  Read::read8(); // Reserved
  header.sourceId = Read::read8() << 8 | Read::read8();
  return true;
}

}  // namespace GigglePixel
