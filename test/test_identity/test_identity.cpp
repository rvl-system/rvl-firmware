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

std::vector<SentPacket>& requests = fake.infrastructureEndpoint.sent;

Bytes idRequest() {
  return rvliPacket(
      UNASSIGNED_DEVICE_ID, RVLI_PACKET_TYPE_ID_ASSIGNMENT, {ID_REQUEST_TYPE});
}

Bytes idReply(uint8_t id) {
  return rvliPacket(
      COORDINATOR_ID, RVLI_PACKET_TYPE_ID_ASSIGNMENT, {ID_REPLY_TYPE, id});
}

void loopAt(uint32_t time) {
  fake.clock = time;
  rvl::loop();
}

// A link-down iteration forgets the ID and pins the retry timer to now, as a
// board's does while it's disconnected
void dropLinkAt(uint32_t time) {
  rvl::setLinkUpState(false);
  loopAt(time);
}

void setUp() {
  dropLinkAt(fake.clock);
  fake.infrastructureEndpoint.sent.clear();
  fake.animationEndpoint.sent.clear();
  fake.randomValues.clear();
  fake.output.clear();
}

void tearDown() {}

void test_unassigned_while_link_down() {
  loopAt(fake.clock + 5000);
  TEST_ASSERT_EQUAL(UNASSIGNED_DEVICE_ID, rvl::getDeviceId());
  TEST_ASSERT_FALSE(rvl::isConnected());
  TEST_ASSERT_EQUAL(0, requests.size());
}

void test_first_request_is_immediate_and_only_on_infrastructure() {
  rvl::setLinkUpState(true);
  loopAt(fake.clock);
  TEST_ASSERT_EQUAL(1, requests.size());
  TEST_ASSERT(requests[0].destination == Destination::Coordinator);
  TEST_ASSERT_PACKET(idRequest(), requests[0].bytes);
  TEST_ASSERT_EQUAL(0, fake.animationEndpoint.sent.size());
  TEST_ASSERT_TRUE(fake.logged("Requesting device ID"));
}

void retriesWithinJitterBand(uint32_t start) {
  dropLinkAt(start);
  fake.randomValues = {0, 249};
  rvl::setLinkUpState(true);
  loopAt(start);
  loopAt(start + 999);
  TEST_ASSERT_EQUAL(1, requests.size());
  loopAt(start + 1000);
  TEST_ASSERT_EQUAL(2, requests.size());
  loopAt(start + 1000 + 1248);
  TEST_ASSERT_EQUAL(2, requests.size());
  loopAt(start + 1000 + 1249);
  TEST_ASSERT_EQUAL(3, requests.size());
}

void test_retries_wait_1000_ms_plus_up_to_250_ms_of_jitter() {
  retriesWithinJitterBand(fake.clock);
}

// A timer zeroed on link down, rather than pinned to now, reads as the future
// here and never requests
void test_retries_after_2_to_the_31_ms_of_uptime() {
  retriesWithinJitterBand(0x90000000);
}

void test_requested_is_logged_once_per_link_up() {
  rvl::setLinkUpState(true);
  loopAt(fake.clock);
  loopAt(fake.clock + 1000);
  TEST_ASSERT_EQUAL(2, requests.size());
  size_t first = fake.output.find("Requesting device ID");
  TEST_ASSERT_NOT_EQUAL(std::string::npos, first);
  TEST_ASSERT_EQUAL(std::string::npos,
      fake.output.find("Requesting device ID", first + 1));
}

void test_a_reply_stores_the_id_and_stops_requests() {
  rvl::setLinkUpState(true);
  loopAt(fake.clock);
  fake.infrastructureEndpoint.receive(idReply(42));
  loopAt(fake.clock + 1);
  TEST_ASSERT_EQUAL(42, rvl::getDeviceId());
  TEST_ASSERT_TRUE(rvl::isConnected());
  TEST_ASSERT_TRUE(fake.logged("Assigned device ID 42"));
  loopAt(fake.clock + 10000);
  TEST_ASSERT_EQUAL(1, requests.size());
}

void test_a_lost_reply_is_recovered_by_the_next_retry() {
  uint32_t start = fake.clock;
  rvl::setLinkUpState(true);
  loopAt(start);
  loopAt(start + 1000);
  TEST_ASSERT_EQUAL(2, requests.size());
  fake.infrastructureEndpoint.receive(idReply(42));
  loopAt(start + 1001);
  TEST_ASSERT_EQUAL(42, rvl::getDeviceId());
}

void test_a_reply_with_an_id_of_240_or_more_is_ignored() {
  rvl::setLinkUpState(true);
  loopAt(fake.clock);
  fake.infrastructureEndpoint.receive(idReply(NUM_DEVICE_IDS));
  loopAt(fake.clock + 1);
  TEST_ASSERT_EQUAL(UNASSIGNED_DEVICE_ID, rvl::getDeviceId());
}

void test_the_id_is_forgotten_on_link_down_and_requested_again() {
  rvl::setLinkUpState(true);
  loopAt(fake.clock);
  fake.infrastructureEndpoint.receive(idReply(42));
  loopAt(fake.clock + 1);
  dropLinkAt(fake.clock + 1);
  TEST_ASSERT_EQUAL(UNASSIGNED_DEVICE_ID, rvl::getDeviceId());
  TEST_ASSERT_FALSE(rvl::isConnected());
  rvl::setLinkUpState(true);
  loopAt(fake.clock + 1);
  TEST_ASSERT_EQUAL(2, requests.size());
}

int main() {
  rvl::init(&fake);
  UNITY_BEGIN();
  RUN_TEST(test_unassigned_while_link_down);
  RUN_TEST(test_first_request_is_immediate_and_only_on_infrastructure);
  RUN_TEST(test_retries_wait_1000_ms_plus_up_to_250_ms_of_jitter);
  RUN_TEST(test_retries_after_2_to_the_31_ms_of_uptime);
  RUN_TEST(test_requested_is_logged_once_per_link_up);
  RUN_TEST(test_a_reply_stores_the_id_and_stops_requests);
  RUN_TEST(test_a_lost_reply_is_recovered_by_the_next_retry);
  RUN_TEST(test_a_reply_with_an_id_of_240_or_more_is_ignored);
  RUN_TEST(test_the_id_is_forgotten_on_link_down_and_requested_again);
  return UNITY_END();
}
