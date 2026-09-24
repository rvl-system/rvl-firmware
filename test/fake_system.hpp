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

#ifndef TEST_FAKE_SYSTEM_H_
#define TEST_FAKE_SYSTEM_H_

#include <deque>
#include <rvl.hpp>
#include <stdint.h>
#include <string>
#include <vector>

typedef std::vector<uint8_t> Bytes;

enum class Destination { Channel, Broadcast, Coordinator };

struct SentPacket {
  Destination destination;
  Bytes bytes;
};

struct QueuedPacket {
  Bytes bytes;
  uint32_t arrivalTime;
};

// The half both protocols share, as UdpEndpoint is in the transports: the same
// big-endian byte order, and reads past the end return 0xFF
template <class Base> class FakeEndpoint : public Base {
public:
  std::deque<QueuedPacket> inbound;
  std::vector<SentPacket> sent;
  uint32_t readsPastEnd = 0;
  // Packets the library moved past without calling endRead()
  uint32_t unendedReads = 0;

  void receive(const Bytes& bytes, uint32_t arrivalTime = 0) {
    inbound.push_back({bytes, arrivalTime});
  }

  void write8(uint8_t data) override {
    pending.bytes.push_back(data);
  }

  void write16(uint16_t data) override {
    write8(data >> 8);
    write8(data & 0xFF);
  }

  void write32(uint32_t data) override {
    write16(data >> 16);
    write16(data & 0xFFFF);
  }

  void write(uint8_t* data, uint16_t length) override {
    pending.bytes.insert(pending.bytes.end(), data, data + length);
  }

  void endWrite() override {
    sent.push_back(pending);
  }

  uint16_t parsePacket() override {
    if (reading) {
      unendedReads++;
      reading = false;
    }
    if (inbound.empty()) {
      return 0;
    }
    current = inbound.front();
    inbound.pop_front();
    cursor = 0;
    reading = true;
    return current.bytes.size();
  }

  uint8_t read8() override {
    if (!reading || cursor >= current.bytes.size()) {
      readsPastEnd++;
      return 0xFF;
    }
    return current.bytes[cursor++];
  }

  uint16_t read16() override {
    uint16_t high = read8();
    return (high << 8) | read8();
  }

  uint32_t read32() override {
    uint32_t high = read16();
    return (high << 16) | read16();
  }

  void read(uint8_t* buffer, uint16_t length) override {
    for (uint16_t i = 0; i < length; i++) {
      buffer[i] = read8();
    }
  }

  void endRead() override {
    reading = false;
  }

protected:
  void beginPacket(Destination destination) {
    pending = {destination, {}};
  }

  QueuedPacket current;
  bool reading = false;

private:
  SentPacket pending;
  size_t cursor = 0;
};

class FakeAnimation : public FakeEndpoint<rvl::System::Animation> {
public:
  void beginChannelWrite() override {
    beginPacket(Destination::Channel);
  }
};

class FakeInfrastructure : public FakeEndpoint<rvl::System::Infrastructure> {
public:
  void beginBroadcastWrite() override {
    beginPacket(Destination::Broadcast);
  }

  void beginCoordinatorWrite() override {
    beginPacket(Destination::Coordinator);
  }

  uint32_t packetArrivalTime() override {
    return reading ? current.arrivalTime : UINT32_MAX;
  }
};

class FakeSystem : public rvl::System {
public:
  FakeAnimation animationEndpoint;
  FakeInfrastructure infrastructureEndpoint;
  // Not 0, which NetworkState reads as "never"
  uint32_t clock = 1000;
  bool linkUp = false;
  // Returned by random() in order, then 0
  std::deque<uint32_t> randomValues;
  std::string output;

  Animation& animation() override {
    return animationEndpoint;
  }

  Infrastructure& infrastructure() override {
    return infrastructureEndpoint;
  }

  bool isLinkUp() override {
    return linkUp;
  }

  void loop() override {}

  uint32_t localClock() override {
    return clock;
  }

  uint32_t random() override {
    if (randomValues.empty()) {
      return 0;
    }
    uint32_t value = randomValues.front();
    randomValues.pop_front();
    return value;
  }

  void print(const char* str) override {
    output += str;
  }

  void println(const char* str) override {
    output += str;
    output += "\n";
  }

  bool logged(const char* text) {
    return output.find(text) != std::string::npos;
  }
};

// One per test program, as the transports have one system each
extern FakeSystem fake;

#endif // TEST_FAKE_SYSTEM_H_
