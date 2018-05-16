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
#include <WiFiUdp.h>
#include "./messaging/udp_transport.h"
#include "../config.h"

namespace UDPTransport {

WiFiUDP* udp;

UDPTransport::UDPTransport(WiFiUDP* newUDP) {
  udp = newUDP;
}

void UDPTransport::beginWrite() {
  udp->beginPacket(GATEWAY, SERVER_PORT);
}

void UDPTransport::write8(uint8 data) {
  udp->write(data);
}

void UDPTransport::write16(uint16 data) {
  udp->write(data >> 8);
  udp->write(data & 0xFF);
}

void UDPTransport::write32(uint32 data) {
  udp->write(data >> 24);
  udp->write(data >> 16 & 0xFF);
  udp->write(data >> 8 & 0xFF);
  udp->write(data & 0xFF);
}

void UDPTransport::write(byte* data, uint16 length) {
  udp->write(data, length);
}

void UDPTransport::endWrite() {
  udp->endPacket();
}

uint8 UDPTransport::read8() {
  return udp->read();
}

uint16 UDPTransport::read16() {
  uint16 val = 0;
  val |= udp->read() << 8;
  val |= udp->read();
  return val;
}

uint32 UDPTransport::read32() {
  uint32 val = 0;
  val |= udp->read() << 24;
  val |= udp->read() << 16;
  val |= udp->read() << 8;
  val |= udp->read();
  return val;
}

void UDPTransport::read(uint8* buffer, int length) {
  udp->read(buffer, length);
}

}  // namespace UDPTransport
