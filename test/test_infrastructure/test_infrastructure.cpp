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

#define COORDINATOR_ID 1
#define LOCAL_ID 10
#define PEER_ID 20

FakeInfrastructure& infrastructure = fake.infrastructureEndpoint;

// An ID reply changes the ID even when the node holds one, so it shows
// whether a packet got past the container
Bytes idReply(uint8_t source, uint8_t id) {
  return rvliPacket(
      source, RVLI_PACKET_TYPE_ID_ASSIGNMENT, {ID_REPLY_TYPE, id});
}

Bytes reference(uint16_t id) {
  return rvliPacket(COORDINATOR_ID, RVLI_PACKET_TYPE_CLOCK_SYNC,
      PacketWriter().u8(1).u16(id).u8(0).u8(1).u8(0).bytes);
}

void deliver(const Bytes& packet) {
  infrastructure.receive(packet, fake.clock);
  rvl::loop();
}

size_t broadcastsSent() {
  size_t count = 0;
  for (auto& packet : infrastructure.sent) {
    if (packet.destination == Destination::Broadcast) {
      count++;
    }
  }
  return count;
}

void setUp() {
  rvl::setLinkUpState(true);
  rvl::setDeviceId(LOCAL_ID);
  infrastructure.sent.clear();
  fake.output.clear();
}

// Every path through the dispatcher ends the read, and a well-formed packet is
// never read past its end
void tearDown() {
  TEST_ASSERT_EQUAL(0, infrastructure.unendedReads);
  TEST_ASSERT_EQUAL(0, infrastructure.readsPastEnd);
}

void test_a_reply_from_the_coordinator_is_accepted() {
  deliver(idReply(COORDINATOR_ID, 42));
  TEST_ASSERT_EQUAL(42, rvl::getDeviceId());
}

void test_a_bad_signature_is_dropped() {
  Bytes packet = idReply(COORDINATOR_ID, 42);
  packet[3] = 'A';
  deliver(packet);
  TEST_ASSERT_EQUAL(LOCAL_ID, rvl::getDeviceId());
}

void test_another_version_is_dropped_and_logged() {
  Bytes packet = idReply(COORDINATOR_ID, 42);
  packet[4] = PROTOCOL_VERSION + 1;
  deliver(packet);
  TEST_ASSERT_EQUAL(LOCAL_ID, rvl::getDeviceId());
  TEST_ASSERT_TRUE(fake.logged("unsupported RVLI protocol version"));
}

void test_the_nodes_own_packets_are_dropped() {
  deliver(idReply(LOCAL_ID, 42));
  TEST_ASSERT_EQUAL(LOCAL_ID, rvl::getDeviceId());
}

void test_sources_of_240_and_up_are_dropped() {
  deliver(idReply(NUM_DEVICE_IDS, 42));
  deliver(idReply(254, 42));
  TEST_ASSERT_EQUAL(LOCAL_ID, rvl::getDeviceId());
}

// A peer's ID request carries 255, which the container drops before the type
// is looked at. A reply is the probe, since a request has no effect either way
void test_id_assignment_from_an_unassigned_source_is_dropped() {
  deliver(idReply(UNASSIGNED_DEVICE_ID, 42));
  TEST_ASSERT_EQUAL(LOCAL_ID, rvl::getDeviceId());
}

// Checked before the ID byte, which a request doesn't have
void test_a_request_with_a_legal_source_is_ignored() {
  deliver(rvliPacket(
      PEER_ID, RVLI_PACKET_TYPE_ID_ASSIGNMENT, {ID_REQUEST_TYPE}));
  TEST_ASSERT_EQUAL(LOCAL_ID, rvl::getDeviceId());
  TEST_ASSERT_EQUAL(0, infrastructure.readsPastEnd);
}

void test_clock_sync_is_accepted_while_connected() {
  deliver(reference(300));
  TEST_ASSERT_EQUAL(1, broadcastsSent());
}

// Without the gate, the node would broadcast an observation with source 255
void test_clock_sync_is_discarded_while_the_node_has_no_id() {
  rvl::setDeviceId(UNASSIGNED_DEVICE_ID);
  deliver(reference(330));
  TEST_ASSERT_EQUAL(0, broadcastsSent());
}

void test_an_unknown_packet_type_is_logged() {
  deliver(rvliPacket(COORDINATOR_ID, 9));
  TEST_ASSERT_TRUE(fake.logged("Received unknown RVLI packet type 9"));
}

int main() {
  rvl::init(&fake);
  UNITY_BEGIN();
  RUN_TEST(test_a_reply_from_the_coordinator_is_accepted);
  RUN_TEST(test_a_bad_signature_is_dropped);
  RUN_TEST(test_another_version_is_dropped_and_logged);
  RUN_TEST(test_the_nodes_own_packets_are_dropped);
  RUN_TEST(test_sources_of_240_and_up_are_dropped);
  RUN_TEST(test_id_assignment_from_an_unassigned_source_is_dropped);
  RUN_TEST(test_a_request_with_a_legal_source_is_ignored);
  RUN_TEST(test_clock_sync_is_accepted_while_connected);
  RUN_TEST(test_clock_sync_is_discarded_while_the_node_has_no_id);
  RUN_TEST(test_an_unknown_packet_type_is_logged);
  return UNITY_END();
}
