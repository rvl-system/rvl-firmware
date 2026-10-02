/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

This file is part of RVL Firmware.

RVL Firmware is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RVL Firmware is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RVL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "packets.hpp"
#include <rvl/config.hpp>

PacketWriter& PacketWriter::u8(uint8_t value) {
  bytes.push_back(value);
  return *this;
}

PacketWriter& PacketWriter::u16(uint16_t value) {
  return u8(value >> 8).u8(value & 0xFF);
}

PacketWriter& PacketWriter::u32(uint32_t value) {
  return u16(value >> 16).u16(value & 0xFFFF);
}

uint32_t readU32(const Bytes& bytes, size_t offset) {
  return static_cast<uint32_t>(bytes[offset]) << 24 |
      static_cast<uint32_t>(bytes[offset + 1]) << 16 |
      static_cast<uint32_t>(bytes[offset + 2]) << 8 | bytes[offset + 3];
}

Bytes rvlaPacket(
    uint8_t source, uint8_t packetType, uint8_t channel, const Bytes& payload) {
  Bytes packet = {
      'R', 'V', 'L', 'A', RVLA_VERSION, source, packetType, channel, 0};
  packet.insert(packet.end(), payload.begin(), payload.end());
  return packet;
}

Bytes rvliPacket(uint8_t source, uint8_t packetType, const Bytes& payload) {
  Bytes packet = {'R', 'V', 'L', 'I', RVLI_VERSION, source, packetType, 0};
  packet.insert(packet.end(), payload.begin(), payload.end());
  return packet;
}
