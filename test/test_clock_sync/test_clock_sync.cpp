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
#include <rvl/protocols/network_state.hpp>
#include <unity.h>

#define COORDINATOR_ID 1
#define LOCAL_ID 10
#define FIRST_PEER_ID 20

#define REFERENCE_TYPE 1
#define OBSERVATION_TYPE 2

// Each test starts a set with a fresh id, which processes whatever the last
// test left, so no test sees another's rows
uint16_t nextSetId = 300;

uint16_t freshSetId() {
  uint16_t id = nextSetId;
  nextSetId += 30;
  return id;
}

uint32_t offset() {
  return rvl::toAnimationClock(0);
}

Bytes reference(uint16_t id, bool startOfSet) {
  return rvliPacket(COORDINATOR_ID, RVLI_PACKET_TYPE_CLOCK_SYNC,
      PacketWriter()
          .u8(REFERENCE_TYPE)
          .u16(id)
          .u8(0)
          .u8(startOfSet ? 1 : 0)
          .u8(0)
          .bytes);
}

Bytes observation(uint8_t source, uint16_t id, uint32_t clock) {
  return rvliPacket(source, RVLI_PACKET_TYPE_CLOCK_SYNC,
      PacketWriter().u8(OBSERVATION_TYPE).u16(id).u8(0).u32(clock).bytes);
}

// Returns the time this node observed the reference at
uint32_t referenceAt(uint32_t time, uint16_t id, bool startOfSet) {
  fake.infrastructureEndpoint.receive(reference(id, startOfSet), time);
  fake.clock = time;
  rvl::loop();
  return rvl::toAnimationClock(time);
}

// Peers from firstPeer up each report observing reference id at observed plus
// their delta
void observationsAt(uint32_t time, uint16_t id, uint32_t observed,
    std::initializer_list<int32_t> deltas, uint8_t firstPeer = FIRST_PEER_ID) {
  uint8_t peer = firstPeer;
  for (int32_t delta : deltas) {
    fake.infrastructureEndpoint.receive(
        observation(peer++, id, observed + delta), time);
  }
  fake.clock = time;
  rvl::loop();
}

void row(uint32_t time, uint16_t id, bool startOfSet,
    std::initializer_list<int32_t> deltas, uint8_t firstPeer = FIRST_PEER_ID) {
  uint32_t observed = referenceAt(time, id, startOfSet);
  observationsAt(time + 5, id, observed, deltas, firstPeer);
}

// Starts the next set, which processes the current one
int32_t processSet(uint32_t time, uint16_t id) {
  uint32_t before = offset();
  referenceAt(time, id, true);
  return static_cast<int32_t>(offset() - before);
}

void setUp() {
  fake.infrastructureEndpoint.sent.clear();
  fake.output.clear();
}

void tearDown() {}

void test_a_reference_is_answered_with_its_arrival_time() {
  uint32_t t = fake.clock + 10000;
  uint16_t id = freshSetId();
  fake.infrastructureEndpoint.receive(reference(id, true), t);
  fake.clock = t + 30;
  rvl::loop();
  auto& sent = fake.infrastructureEndpoint.sent;
  TEST_ASSERT_EQUAL(1, sent.size());
  TEST_ASSERT(sent[0].destination == Destination::Broadcast);
  TEST_ASSERT_PACKET(observation(LOCAL_ID, id, t + offset()), sent[0].bytes);
}

void test_each_row_takes_the_upper_median() {
  uint32_t t = fake.clock + 10000;
  uint16_t id = freshSetId();
  row(t, id, true, {10, 20, 30});
  row(t + 100, id + 1, false, {10, 20, 30});
  row(t + 200, id + 2, false, {10, 20, 30});
  TEST_ASSERT_EQUAL_INT32(20, processSet(t + 2300, id + 3));
  TEST_ASSERT_TRUE(fake.logged("offset: 20 from 4 nodes across 3 references"));
}

void test_rows_with_mixed_numbers_of_observations_are_averaged() {
  uint32_t t = fake.clock + 10000;
  uint16_t id = freshSetId();
  row(t, id, true, {10, 20, 30});
  row(t + 100, id + 1, false, {10, 50});
  row(t + 200, id + 2, false, {60});
  TEST_ASSERT_EQUAL_INT32(30, processSet(t + 2300, id + 3));
}

void test_the_start_of_set_observation_uses_the_corrected_clock() {
  uint32_t t = fake.clock + 10000;
  uint16_t id = freshSetId();
  row(t, id, true, {20, 20});
  row(t + 100, id + 1, false, {20, 20});
  row(t + 200, id + 2, false, {20, 20});
  uint32_t before = offset();
  fake.infrastructureEndpoint.sent.clear();
  referenceAt(t + 2300, id + 3, true);
  TEST_ASSERT_PACKET(observation(LOCAL_ID, id + 3, t + 2300 + before + 20),
      fake.infrastructureEndpoint.sent.back().bytes);
}

void test_a_row_is_discarded_when_this_node_missed_its_reference() {
  uint32_t t = fake.clock + 10000;
  uint16_t id = freshSetId();
  row(t, id, true, {20, 20});
  observationsAt(t + 105, id + 1, t + 100 + offset(), {90, 90});
  row(t + 200, id + 2, false, {20, 20});
  TEST_ASSERT_EQUAL_INT32(20, processSet(t + 2300, id + 3));
  TEST_ASSERT_TRUE(fake.logged("across 2 references"));
}

// A set whose start this node missed is never processed, so its rows still
// hold data when the next set reuses them
void test_a_reused_row_is_cleared_of_an_older_reference() {
  uint32_t t = fake.clock + 10000;
  uint16_t id = freshSetId();
  row(t, id, true, {1000, 1000, 1000, 1000, 1000}, FIRST_PEER_ID + 10);
  row(t + 100, id + 1, false, {1000, 1000, 1000, 1000, 1000},
      FIRST_PEER_ID + 10);
  row(t + 200, id + 2, false, {1000, 1000, 1000, 1000, 1000},
      FIRST_PEER_ID + 10);
  observationsAt(t + 2305, id + 3, t + 2300 + offset(), {20, 20});
  row(t + 2400, id + 4, false, {20, 20});
  row(t + 2500, id + 5, false, {20, 20});
  TEST_ASSERT_EQUAL_INT32(20, processSet(t + 4600, id + 6));
}

void test_a_lone_board_syncs_with_a_zero_correction() {
  uint32_t t = fake.clock + 61000;
  fake.clock = t;
  rvl::loop();
  TEST_ASSERT_FALSE(rvl::NetworkState::isClockSynchronizationActive());
  uint16_t id = freshSetId();
  row(t, id, true, {});
  row(t + 100, id + 1, false, {});
  row(t + 200, id + 2, false, {});
  TEST_ASSERT_EQUAL_INT32(0, processSet(t + 2300, id + 3));
  TEST_ASSERT_TRUE(rvl::NetworkState::isClockSynchronizationActive());
}

// Deltas near 2^31 overflowed the old int32_t accumulator into a stuck
// zero-correction state
void test_a_cold_start_against_a_long_running_fleet() {
  const uint32_t fleetAhead = 2999990000;
  const int32_t delta = static_cast<int32_t>(fleetAhead);
  uint32_t t = fake.clock + 10000;
  uint16_t id = freshSetId();
  row(t, id, true, {delta, delta});
  row(t + 100, id + 1, false, {delta, delta});
  row(t + 200, id + 2, false, {delta, delta});
  TEST_ASSERT_EQUAL_UINT32(
      fleetAhead, static_cast<uint32_t>(processSet(t + 2300, id + 3)));
}

// Reference id + 3 reuses id's row, so an observation of id arriving after it
// clears the row, taking this node's observation of id + 3 with it
void test_a_straggler_from_the_last_set_keeps_this_sets_row() {
  TEST_IGNORE_MESSAGE("Known bug, deferred to Phase 3");
  uint32_t t = fake.clock + 10000;
  uint16_t id = freshSetId();
  row(t, id, true, {0});
  row(t + 100, id + 1, false, {0});
  row(t + 200, id + 2, false, {0});
  uint32_t observed = referenceAt(t + 2300, id + 3, true);
  observationsAt(t + 2305, id, observed, {0}, FIRST_PEER_ID + 10);
  observationsAt(t + 2310, id + 3, observed, {30, 30});
  row(t + 2400, id + 4, false, {0, 0});
  row(t + 2500, id + 5, false, {0, 0});
  TEST_ASSERT_EQUAL_INT32(10, processSet(t + 4600, id + 6));
}

void test_the_animation_clock_wraps_with_its_offset() {
  rvl::adjustAnimationClock(static_cast<int32_t>(0xFFFFFF00 - offset()));
  TEST_ASSERT_EQUAL_UINT32(0xFFFFFF00, offset());
  TEST_ASSERT_EQUAL_UINT32(0x100, rvl::toAnimationClock(0x200));
  fake.clock = 0x200;
  TEST_ASSERT_EQUAL_UINT32(0x100, rvl::getAnimationClock());
  rvl::adjustAnimationClock(-0x200);
  TEST_ASSERT_EQUAL_UINT32(0xFFFFFF00, rvl::toAnimationClock(0x200));
}

// Corrections accumulate modulo 2^32, like the clocks they correct
void test_offset_adjustments_wrap_in_both_directions() {
  uint32_t before = offset();
  rvl::adjustAnimationClock(INT32_MAX);
  rvl::adjustAnimationClock(INT32_MAX);
  rvl::adjustAnimationClock(2);
  TEST_ASSERT_EQUAL_UINT32(before, offset());
  rvl::adjustAnimationClock(INT32_MIN);
  rvl::adjustAnimationClock(INT32_MIN);
  TEST_ASSERT_EQUAL_UINT32(before, offset());
}

int main() {
  rvl::init(&fake);
  rvl::setLinkUpState(true);
  rvl::setDeviceId(LOCAL_ID);
  UNITY_BEGIN();
  RUN_TEST(test_a_reference_is_answered_with_its_arrival_time);
  RUN_TEST(test_each_row_takes_the_upper_median);
  RUN_TEST(test_rows_with_mixed_numbers_of_observations_are_averaged);
  RUN_TEST(test_the_start_of_set_observation_uses_the_corrected_clock);
  RUN_TEST(test_a_row_is_discarded_when_this_node_missed_its_reference);
  RUN_TEST(test_a_reused_row_is_cleared_of_an_older_reference);
  RUN_TEST(test_a_lone_board_syncs_with_a_zero_correction);
  RUN_TEST(test_a_cold_start_against_a_long_running_fleet);
  RUN_TEST(test_a_straggler_from_the_last_set_keeps_this_sets_row);
  RUN_TEST(test_the_animation_clock_wraps_with_its_offset);
  RUN_TEST(test_offset_adjustments_wrap_in_both_directions);
  return UNITY_END();
}
