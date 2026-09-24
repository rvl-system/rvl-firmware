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

#ifndef TEST_PACKETS_H_
#define TEST_PACKETS_H_

#include "fake_system.hpp"
#include <stdint.h>
#include <unity.h>

// Payload fields in wire order, big-endian like the firmware's writers
class PacketWriter {
public:
  Bytes bytes;

  PacketWriter& u8(uint8_t value);
  PacketWriter& u16(uint16_t value);
  PacketWriter& u32(uint32_t value);
};

// Spelled out here rather than built with the library's own header writers, so
// a test can't agree with a wrong writer
Bytes rvlaPacket(uint8_t source, uint8_t packetType, uint8_t channel,
    const Bytes& payload = {});
Bytes rvliPacket(uint8_t source, uint8_t packetType, const Bytes& payload = {});

#define TEST_ASSERT_PACKET(expected, actual)                                   \
  do {                                                                         \
    Bytes expectedBytes = (expected);                                          \
    Bytes actualBytes = (actual);                                              \
    TEST_ASSERT_EQUAL_MESSAGE(                                                 \
        expectedBytes.size(), actualBytes.size(), "packet length");           \
    if (!expectedBytes.empty()) {                                              \
      TEST_ASSERT_EQUAL_HEX8_ARRAY(                                            \
          expectedBytes.data(), actualBytes.data(), expectedBytes.size());     \
    }                                                                          \
  } while (0)

#endif // TEST_PACKETS_H_
