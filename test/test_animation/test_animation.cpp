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

#include "fake_system.hpp"
#include "packets.hpp"
#include <initializer_list>
#include <rvl.hpp>
#include <rvl/config.hpp>
#include <unity.h>

#define LOCAL_ID 10
#define CONTROLLER_ID 20

FakeAnimation& animation = fake.animationEndpoint;

// With no controller adopted yet, the first source an off packet reaches
// NetworkState from becomes the controller, so the animation type shows
// whether a packet got past the container
Bytes off(uint8_t source = CONTROLLER_ID, uint8_t channel = 0) {
  return rvlaPacket(source, PACKET_TYPE_OFF, channel);
}

void deliver(const Bytes& packet) {
  animation.receive(packet);
  rvl::loop();
}

bool isOff() {
  return rvl::getAnimationType() == rvl::AnimationType::Off;
}

void loopAt(uint32_t time) {
  fake.clock = time;
  rvl::loop();
}

uint8_t packetType(const SentPacket& packet) {
  return packet.bytes[6];
}

// Every field different, and the signed ones negative
RVLWaveSettings distinctiveWave() {
  RVLWaveSettings settings;
  settings.timePeriod = 200;
  settings.distancePeriod = 16;
  uint8_t n = 0;
  for (auto& wave : settings.waves) {
    for (auto* channel : {&wave.h, &wave.s, &wave.v, &wave.a}) {
      channel->a = 100 + n;
      channel->b = 200 + n;
      channel->w_t = -1 - n;
      channel->w_x = 1 + n;
      channel->phi = -64 + n;
      n++;
    }
  }
  return settings;
}

// The wire layout: time period, distance period, then each wave's h, s, v and
// a, each as a, b, w_t, w_x, phi
Bytes wavePayload(const RVLWaveSettings& settings) {
  PacketWriter payload;
  payload.u8(settings.timePeriod).u8(settings.distancePeriod);
  for (auto& wave : settings.waves) {
    for (auto* channel : {&wave.h, &wave.s, &wave.v, &wave.a}) {
      payload.u8(channel->a)
          .u8(channel->b)
          .u8(channel->w_t)
          .u8(channel->w_x)
          .u8(channel->phi);
    }
  }
  return payload.bytes;
}

void setUp() {
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  rvl::setLinkUpState(true);
  rvl::setDeviceId(LOCAL_ID);
  RVLWaveSettings wave;
  rvl::setWaveSettings(&wave);
  // A channel change forgets the controller
  rvl::setChannel(1);
  rvl::setChannel(0);
  animation.sent.clear();
  fake.output.clear();
}

// Every path through the dispatcher ends the read, and a well-formed packet is
// never read past its end
void tearDown() {
  TEST_ASSERT_EQUAL(0, animation.unendedReads);
  TEST_ASSERT_EQUAL(0, animation.readsPastEnd);
}

void test_off_from_a_controller_on_the_channel_selects_off() {
  deliver(off());
  TEST_ASSERT_TRUE(isOff());
}

void test_a_bad_signature_is_dropped() {
  Bytes packet = off();
  packet[3] = 'I';
  deliver(packet);
  TEST_ASSERT_FALSE(isOff());
}

void test_another_version_is_dropped_and_logged() {
  Bytes packet = off();
  packet[4] = PROTOCOL_VERSION + 1;
  deliver(packet);
  TEST_ASSERT_FALSE(isOff());
  TEST_ASSERT_TRUE(fake.logged("unsupported RVLA protocol version"));
}

void test_the_nodes_own_packets_are_dropped() {
  deliver(off(LOCAL_ID));
  TEST_ASSERT_FALSE(isOff());
}

void test_sources_of_240_and_up_are_dropped() {
  deliver(off(NUM_DEVICE_IDS));
  deliver(off(254));
  deliver(off(UNASSIGNED_DEVICE_ID));
  TEST_ASSERT_FALSE(isOff());
}

void test_only_the_nodes_channel_is_accepted() {
  rvl::setChannel(3);
  deliver(off(CONTROLLER_ID, 0));
  deliver(off(CONTROLLER_ID, 2));
  TEST_ASSERT_FALSE(isOff());
  deliver(off(CONTROLLER_ID, 3));
  TEST_ASSERT_TRUE(isOff());
}

// The self and channel filters both presuppose an identity
void test_everything_is_discarded_while_the_node_has_no_id() {
  rvl::setDeviceId(UNASSIGNED_DEVICE_ID);
  deliver(off());
  TEST_ASSERT_FALSE(isOff());
}

void test_an_unknown_packet_type_is_logged() {
  deliver(rvlaPacket(CONTROLLER_ID, 9, 0));
  TEST_ASSERT_FALSE(isOff());
  TEST_ASSERT_TRUE(fake.logged("Received unknown RVLA packet type 9"));
}

// A controller's packet fed back to a receiver, which needs a peer's source or
// it drops the packet as its own
void test_a_sent_wave_round_trips() {
  RVLWaveSettings settings = distinctiveWave();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::setWaveSettings(&settings);
  TEST_ASSERT_EQUAL(1, animation.sent.size());
  TEST_ASSERT(animation.sent[0].destination == Destination::Channel);
  TEST_ASSERT_PACKET(rvlaPacket(LOCAL_ID, PACKET_TYPE_WAVE_ANIMATION, 0,
                         wavePayload(settings)),
      animation.sent[0].bytes);

  Bytes packet = animation.sent[0].bytes;
  packet[5] = CONTROLLER_ID;
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  RVLWaveSettings defaults;
  rvl::setWaveSettings(&defaults);
  deliver(packet);
  TEST_ASSERT_EQUAL_MEMORY(
      &settings, rvl::getWaveSettings(), sizeof(RVLWaveSettings));
}

void test_a_sent_off_round_trips_and_a_wave_after_it_restores_wave() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::setOff();
  TEST_ASSERT_EQUAL(1, animation.sent.size());
  TEST_ASSERT_PACKET(
      rvlaPacket(LOCAL_ID, PACKET_TYPE_OFF, 0), animation.sent[0].bytes);

  Bytes packet = animation.sent[0].bytes;
  packet[5] = CONTROLLER_ID;
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  RVLWaveSettings defaults;
  rvl::setWaveSettings(&defaults);
  deliver(packet);
  TEST_ASSERT_TRUE(isOff());

  RVLWaveSettings settings = distinctiveWave();
  deliver(rvlaPacket(CONTROLLER_ID, PACKET_TYPE_WAVE_ANIMATION, 0,
      wavePayload(settings)));
  TEST_ASSERT_FALSE(isOff());
  TEST_ASSERT_EQUAL_MEMORY(
      &settings, rvl::getWaveSettings(), sizeof(RVLWaveSettings));
}

// One sender for every type, so a controller never sends two types at once
void test_the_periodic_sender_repeats_only_the_current_selection() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  uint32_t start = fake.clock;
  rvl::setOff();
  for (uint32_t elapsed = 1000; elapsed <= 4000; elapsed += 1000) {
    loopAt(start + elapsed);
  }
  // The selection, then at least two repeats 2 s apart
  TEST_ASSERT_GREATER_OR_EQUAL(3, animation.sent.size());
  for (auto& packet : animation.sent) {
    TEST_ASSERT_EQUAL(PACKET_TYPE_OFF, packetType(packet));
  }

  animation.sent.clear();
  RVLWaveSettings settings = distinctiveWave();
  rvl::setWaveSettings(&settings);
  for (uint32_t elapsed = 5000; elapsed <= 8000; elapsed += 1000) {
    loopAt(start + elapsed);
  }
  TEST_ASSERT_GREATER_OR_EQUAL(3, animation.sent.size());
  for (auto& packet : animation.sent) {
    TEST_ASSERT_EQUAL(PACKET_TYPE_WAVE_ANIMATION, packetType(packet));
  }
}

void test_a_controller_without_an_id_sends_nothing() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::setDeviceId(UNASSIGNED_DEVICE_ID);
  uint32_t start = fake.clock;
  rvl::setOff();
  for (uint32_t elapsed = 1000; elapsed <= 4000; elapsed += 1000) {
    loopAt(start + elapsed);
  }
  TEST_ASSERT_EQUAL(0, animation.sent.size());
}

void test_a_receiver_never_sends() {
  uint32_t start = fake.clock;
  rvl::setOff();
  for (uint32_t elapsed = 1000; elapsed <= 4000; elapsed += 1000) {
    loopAt(start + elapsed);
  }
  TEST_ASSERT_EQUAL(0, animation.sent.size());
}

int main() {
  rvl::init(&fake);
  UNITY_BEGIN();
  RUN_TEST(test_off_from_a_controller_on_the_channel_selects_off);
  RUN_TEST(test_a_bad_signature_is_dropped);
  RUN_TEST(test_another_version_is_dropped_and_logged);
  RUN_TEST(test_the_nodes_own_packets_are_dropped);
  RUN_TEST(test_sources_of_240_and_up_are_dropped);
  RUN_TEST(test_only_the_nodes_channel_is_accepted);
  RUN_TEST(test_everything_is_discarded_while_the_node_has_no_id);
  RUN_TEST(test_an_unknown_packet_type_is_logged);
  RUN_TEST(test_a_sent_wave_round_trips);
  RUN_TEST(test_a_sent_off_round_trips_and_a_wave_after_it_restores_wave);
  RUN_TEST(test_the_periodic_sender_repeats_only_the_current_selection);
  RUN_TEST(test_a_controller_without_an_id_sends_nothing);
  RUN_TEST(test_a_receiver_never_sends);
  return UNITY_END();
}
