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

void setUp() {
  rvl::setLinkUpState(true);
  rvl::setDeviceId(LOCAL_ID);
  RVLWaveSettings wave;
  rvl::setWaveSettings(&wave);
  // A channel change forgets the controller
  rvl::setChannel(1);
  rvl::setChannel(0);
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
  return UNITY_END();
}
