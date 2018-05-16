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

#ifndef MESSAGING_UDP_TRANSPORT_H_
#define MESSAGING_UDP_TRANSPORT_H_

#include <Arduino.h>
#include "./messaging/stack/transport.h"

namespace UDPTransport {

class UDPTransport : public TransportInterface {
 public:
  explicit UDPTransport(WiFiUDP* udp);
  void beginWrite();
  void write8(uint8 data);
  void write16(uint16 data);
  void write32(uint32 data);
  void write(byte* data, uint16 length);
  void endWrite();

  uint8 read8();
  uint16 read16();
  uint32 read32();
  void read(uint8* buffer, int length);
};

}  // namespace UDPTransport

#endif  // MESSAGING_UDP_TRANSPORT_H_
