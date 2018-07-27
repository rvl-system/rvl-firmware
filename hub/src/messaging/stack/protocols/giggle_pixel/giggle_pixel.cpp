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
#include "./messaging/stack/protocols/giggle_pixel/giggle_pixel.h"
#include "./messaging/stack/protocols/giggle_pixel/palette.h"
#include "./messaging/stack/protocols/giggle_pixel/wave.h"
#include "./messaging/stack/transport.h"
#include "../../../../config.h"  // Why does this one single file require ".." but none of the others do?
#include "./codes.h"
#include "./state.h"
#include "./util/logging.h"

namespace GigglePixel {

const uint8 protocolVersion = 1;

TransportInterface* transport;

void init(TransportInterface* newTransport) {
  transport = newTransport;
  Wave::init(newTransport);
  Palette::init(newTransport);
}

void loop() {
  Wave::loop();
}

void setClientId(uint16 id);
void broadcastHeader(uint8 packetType, uint8 priority, uint16 length);

void parsePacket() {
  Logging::debug("Parsing GigglePixel packet");
  uint8 protocolVersion = transport->read8();
  if (protocolVersion != protocolVersion) {
    Logging::error("Received unsupported GigglePixel protocol version packet");
    return;
  }
  transport->read16();  // length
  uint8 packetType = transport->read8();
  transport->read8();  // priority
  transport->read8();  // Reserved
  transport->read16();  // sourceId

  // Ignore our own broadcast packets
  if (State::getSettings()->mode == Codes::Mode::Controller) {
    return;
  }

  switch (packetType) {
    case Codes::GigglePixelPacketTypes::Wave:
      Wave::parsePacket();
      break;
    default:
      Logging::error("Unsupported packet type received: %d", packetType);
  }
}

void broadcastHeader(uint8 packetType, uint8 priority, uint16 length) {
  uint8* signature = const_cast<uint8*>(reinterpret_cast<const uint8*>("GLPX"));
  transport->write(signature, sizeof(uint8) * 4);
  transport->write8(protocolVersion);
  transport->write16(length);
  transport->write8(packetType);
  transport->write8(priority);
  transport->write8(0);  // reserved
  transport->write16(State::getSettings()->id);
}

}  // namespace GigglePixel
