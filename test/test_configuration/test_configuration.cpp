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
#include "golden_packets.hpp"
#include "packets.hpp"
#include <rvl.hpp>
#include <rvl/config.hpp>
#include <string.h>
#include <string>
#include <unity.h>

#define LOCAL_ID 10
#define CONFIGURER_ID 20
#define OTHER_ID 30

#define ALL_CAPABILITIES 0x3f

FakeInfrastructure& infrastructure = fake.infrastructureEndpoint;

// One count per property's event, indexed as the wire numbers them
uint32_t events[NUM_PROPERTIES];

void countChannel() {
  events[0]++;
}
void countBrightness() {
  events[1]++;
}
void countPixelCount() {
  events[2]++;
}
void countColorOrder() {
  events[3]++;
}
void countStatusLedBrightness() {
  events[4]++;
}
void countNickname() {
  events[5]++;
}

void deliver(const Bytes& packet) {
  infrastructure.receive(packet, fake.clock);
  rvl::loop();
}

Bytes query(uint8_t destination) {
  return rvliPacket(CONFIGURER_ID, RVLI_PACKET_TYPE_QUERY, {}, destination);
}

Bytes identify(uint8_t destination) {
  return rvliPacket(CONFIGURER_ID, RVLI_PACKET_TYPE_IDENTIFY, {}, destination);
}

Bytes configure(uint8_t property, const Bytes& value) {
  Bytes payload = {property};
  payload.insert(payload.end(), value.begin(), value.end());
  return rvliPacket(
      CONFIGURER_ID, RVLI_PACKET_TYPE_CONFIGURE, payload, LOCAL_ID);
}

// Zero padded to the wire's width
Bytes nicknameBytes(const char* nickname) {
  Bytes bytes(NICKNAME_LENGTH, 0);
  memcpy(bytes.data(), nickname, strlen(nickname));
  return bytes;
}

struct Status {
  uint8_t destination;
  uint8_t capabilities;
  uint8_t mode;
  uint8_t channel;
  uint8_t brightness;
  uint16_t pixelCount;
  uint8_t colorOrder;
  uint8_t statusLedBrightness;
  std::string nickname;
};

Status readStatus(const Bytes& bytes) {
  TEST_ASSERT_EQUAL(32, bytes.size());
  TEST_ASSERT_EQUAL(RVLI_PACKET_TYPE_STATUS, bytes[6]);
  std::string nickname(bytes.begin() + 16, bytes.begin() + 32);
  nickname = nickname.substr(0, nickname.find('\0'));
  return {bytes[7], bytes[8], bytes[9], bytes[10], bytes[11],
      static_cast<uint16_t>(bytes[12] << 8 | bytes[13]), bytes[14], bytes[15],
      nickname};
}

// The one status sent, read back
Status sentStatus() {
  TEST_ASSERT_EQUAL(1, infrastructure.sent.size());
  TEST_ASSERT(infrastructure.sent[0].destination == Destination::Broadcast);
  return readStatus(infrastructure.sent[0].bytes);
}

void setUp() {
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  rvl::Scenes::reset();
  rvl::setCapabilities(ALL_CAPABILITIES);
  rvl::setMaxPixelCount(300);
  rvl::setChannel(0);
  rvl::setBrightness(50);
  rvl::setPixelCount(300);
  rvl::setColorOrder(rvl::ColorOrder::GRB);
  rvl::setStatusLedBrightness(60);
  rvl::setNickname("Mini 1");
  memset(events, 0, sizeof(events));
  infrastructure.sent.clear();
  fake.animationEndpoint.sent.clear();
  fake.output.clear();
}

// Every path through the dispatcher ends the read, and no packet here is read
// past its end
void tearDown() {
  TEST_ASSERT_EQUAL(0, infrastructure.unendedReads);
  TEST_ASSERT_EQUAL(0, infrastructure.readsPastEnd);
}

uint32_t eventsFired() {
  uint32_t total = 0;
  for (uint32_t count : events) {
    total += count;
  }
  return total;
}

void test_a_query_is_answered_with_this_nodes_status() {
  const Bytes expected = {'R', 'V', 'L', 'I', RVLI_VERSION, LOCAL_ID,
      RVLI_PACKET_TYPE_STATUS, CONFIGURER_ID, ALL_CAPABILITIES, 1, 0, 50, 0x01,
      0x2c, 2, 60, 'M', 'i', 'n', 'i', ' ', '1', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  deliver(query(LOCAL_ID));
  TEST_ASSERT_EQUAL(1, infrastructure.sent.size());
  TEST_ASSERT(infrastructure.sent[0].destination == Destination::Broadcast);
  TEST_ASSERT_PACKET(expected, infrastructure.sent[0].bytes);
  deliver(query(EVERY_DEVICE_ID));
  TEST_ASSERT_EQUAL(2, infrastructure.sent.size());
  TEST_ASSERT_PACKET(expected, infrastructure.sent[1].bytes);
  TEST_ASSERT_EQUAL(0, eventsFired());
}

void test_a_query_for_another_node_is_not_answered() {
  deliver(query(OTHER_ID));
  TEST_ASSERT_EQUAL(0, infrastructure.sent.size());
}

// A configure applies its property, fires that property's event once, and is
// answered with the new value; the same configure again changes nothing and
// fires nothing, and is answered all the same
void applyAndRepeat(uint8_t property, const Bytes& value) {
  deliver(configure(property, value));
  TEST_ASSERT_EQUAL(1, events[property]);
  TEST_ASSERT_EQUAL(1, eventsFired());
  TEST_ASSERT_EQUAL(1, infrastructure.sent.size());
  Bytes first = infrastructure.sent[0].bytes;
  infrastructure.sent.clear();
  deliver(configure(property, value));
  TEST_ASSERT_EQUAL(1, eventsFired());
  TEST_ASSERT_EQUAL(1, infrastructure.sent.size());
  TEST_ASSERT_PACKET(first, infrastructure.sent[0].bytes);
}

void test_a_channel_is_applied_and_answered() {
  applyAndRepeat(0, {1});
  TEST_ASSERT_EQUAL(1, rvl::getChannel());
  TEST_ASSERT_EQUAL(1, sentStatus().channel);
}

void test_a_brightness_is_applied_and_answered() {
  applyAndRepeat(1, {200});
  TEST_ASSERT_EQUAL(200, rvl::getBrightness());
  TEST_ASSERT_EQUAL(200, sentStatus().brightness);
}

void test_a_pixel_count_is_applied_and_answered() {
  applyAndRepeat(2, {0, 240});
  TEST_ASSERT_EQUAL(240, rvl::getPixelCount());
  TEST_ASSERT_EQUAL(240, sentStatus().pixelCount);
}

void test_a_color_order_is_applied_and_answered() {
  applyAndRepeat(3, {5});
  TEST_ASSERT(rvl::getColorOrder() == rvl::ColorOrder::BGR);
  TEST_ASSERT_EQUAL(5, sentStatus().colorOrder);
}

void test_a_status_led_brightness_is_applied_and_answered() {
  applyAndRepeat(4, {30});
  TEST_ASSERT_EQUAL(30, rvl::getStatusLedBrightness());
  TEST_ASSERT_EQUAL(30, sentStatus().statusLedBrightness);
}

void test_a_nickname_is_applied_and_answered() {
  applyAndRepeat(5, nicknameBytes("left bar"));
  TEST_ASSERT_EQUAL_STRING("left bar", rvl::getNickname());
  TEST_ASSERT_EQUAL_STRING("left bar", sentStatus().nickname.c_str());
}

void test_a_property_this_node_lacks_is_left_alone_and_answered() {
  rvl::setCapabilities(ALL_CAPABILITIES &
      ~rvl::capabilityBit(rvl::Property::StatusLedBrightness));
  deliver(configure(4, {90}));
  TEST_ASSERT_EQUAL(0, eventsFired());
  TEST_ASSERT_EQUAL(60, rvl::getStatusLedBrightness());
  Status status = sentStatus();
  TEST_ASSERT_EQUAL(60, status.statusLedBrightness);
  TEST_ASSERT_EQUAL(ALL_CAPABILITIES & ~0x10, status.capabilities);
  TEST_ASSERT_FALSE(fake.logged("ignoring"));
}

void test_a_pixel_count_past_the_buffers_lands_as_the_maximum() {
  deliver(configure(2, {0x03, 0xe8}));
  TEST_ASSERT_EQUAL(300, rvl::getPixelCount());
  TEST_ASSERT_EQUAL(0, eventsFired());
  TEST_ASSERT_EQUAL(300, sentStatus().pixelCount);
  rvl::setPixelCount(240);
  infrastructure.sent.clear();
  deliver(configure(2, {0x03, 0xe8}));
  TEST_ASSERT_EQUAL(300, rvl::getPixelCount());
  TEST_ASSERT_EQUAL(2, events[2]);
  TEST_ASSERT_EQUAL(300, sentStatus().pixelCount);
}

// Only a bug sends one, since rvl-node checks before sending, so the packet is
// dropped whole rather than answered with the old value
void test_a_bad_channel_color_order_or_property_drops_the_packet() {
  deliver(configure(0, {8}));
  TEST_ASSERT_TRUE(fake.logged("Received channel 8, ignoring"));
  deliver(configure(3, {6}));
  TEST_ASSERT_TRUE(fake.logged("Received color order 6, ignoring"));
  deliver(configure(6, {1}));
  TEST_ASSERT_TRUE(fake.logged("Received configure for unknown property 6"));
  TEST_ASSERT_EQUAL(0, infrastructure.sent.size());
  TEST_ASSERT_EQUAL(0, eventsFired());
  TEST_ASSERT_EQUAL(0, rvl::getChannel());
  TEST_ASSERT(rvl::getColorOrder() == rvl::ColorOrder::GRB);
}

void test_a_full_length_nickname_comes_back_terminated() {
  deliver(configure(5, nicknameBytes("sixteen chars ok")));
  TEST_ASSERT_EQUAL(16, strlen(rvl::getNickname()));
  TEST_ASSERT_EQUAL_STRING("sixteen chars ok", rvl::getNickname());
  TEST_ASSERT_EQUAL_STRING("sixteen chars ok", sentStatus().nickname.c_str());
}

void test_identify_answers_and_lasts_five_seconds() {
  TEST_ASSERT_FALSE(rvl::isIdentifying());
  uint32_t t = fake.clock;
  deliver(identify(LOCAL_ID));
  TEST_ASSERT_EQUAL(1, infrastructure.sent.size());
  TEST_ASSERT_EQUAL(0, eventsFired());
  TEST_ASSERT_TRUE(rvl::isIdentifying());
  fake.clock = t + IDENTIFY_DURATION - 1;
  TEST_ASSERT_TRUE(rvl::isIdentifying());
  fake.clock = t + IDENTIFY_DURATION;
  TEST_ASSERT_FALSE(rvl::isIdentifying());
  // Every loop pins the end time to now once over, so however long the board
  // runs it never reads as the future again. Without the pin the second step
  // here, 2^32 - 2 ms after the end, would read as 2 ms before it
  rvl::loop();
  fake.clock += 0x7fffffff;
  TEST_ASSERT_FALSE(rvl::isIdentifying());
  rvl::loop();
  fake.clock += 0x7fffffff;
  TEST_ASSERT_FALSE(rvl::isIdentifying());
}

void test_an_identify_for_another_node_is_not_taken() {
  deliver(identify(OTHER_ID));
  TEST_ASSERT_EQUAL(0, infrastructure.sent.size());
  TEST_ASSERT_FALSE(rvl::isIdentifying());
}

// Another board's reply to whoever asked it
void test_a_status_from_another_node_changes_nothing() {
  Bytes payload = {ALL_CAPABILITIES, 0, 7, 255, 0x00, 0x18, 5, 1};
  Bytes nickname = nicknameBytes("other");
  payload.insert(payload.end(), nickname.begin(), nickname.end());
  deliver(rvliPacket(OTHER_ID, RVLI_PACKET_TYPE_STATUS, payload, LOCAL_ID));
  TEST_ASSERT_EQUAL(0, infrastructure.sent.size());
  TEST_ASSERT_EQUAL(0, eventsFired());
  TEST_ASSERT_EQUAL(0, rvl::getChannel());
  TEST_ASSERT_EQUAL_STRING("Mini 1", rvl::getNickname());
  TEST_ASSERT_FALSE(fake.logged("unknown"));
}

void test_rvl_nodes_configure_packet_parses() {
  rvl::setNickname("");
  memset(events, 0, sizeof(events));
  deliver(GOLDEN_CONFIGURE_PACKET);
  TEST_ASSERT_EQUAL_STRING("Mini 1", rvl::getNickname());
  TEST_ASSERT_EQUAL(1, events[5]);
  TEST_ASSERT_EQUAL_STRING("Mini 1", sentStatus().nickname.c_str());
}

int main() {
  rvl::init(&fake);
  rvl::setLinkUpState(true);
  rvl::setDeviceId(LOCAL_ID);
  rvl::on(EVENT_CHANNEL_UPDATED, countChannel);
  rvl::on(EVENT_BRIGHTNESS_UPDATED, countBrightness);
  rvl::on(EVENT_PIXEL_COUNT_UPDATED, countPixelCount);
  rvl::on(EVENT_COLOR_ORDER_UPDATED, countColorOrder);
  rvl::on(EVENT_STATUS_LED_BRIGHTNESS_UPDATED, countStatusLedBrightness);
  rvl::on(EVENT_NICKNAME_UPDATED, countNickname);
  UNITY_BEGIN();
  RUN_TEST(test_a_query_is_answered_with_this_nodes_status);
  RUN_TEST(test_a_query_for_another_node_is_not_answered);
  RUN_TEST(test_a_channel_is_applied_and_answered);
  RUN_TEST(test_a_brightness_is_applied_and_answered);
  RUN_TEST(test_a_pixel_count_is_applied_and_answered);
  RUN_TEST(test_a_color_order_is_applied_and_answered);
  RUN_TEST(test_a_status_led_brightness_is_applied_and_answered);
  RUN_TEST(test_a_nickname_is_applied_and_answered);
  RUN_TEST(test_a_property_this_node_lacks_is_left_alone_and_answered);
  RUN_TEST(test_a_pixel_count_past_the_buffers_lands_as_the_maximum);
  RUN_TEST(test_a_bad_channel_color_order_or_property_drops_the_packet);
  RUN_TEST(test_a_full_length_nickname_comes_back_terminated);
  RUN_TEST(test_identify_answers_and_lasts_five_seconds);
  RUN_TEST(test_an_identify_for_another_node_is_not_taken);
  RUN_TEST(test_a_status_from_another_node_changes_nothing);
  RUN_TEST(test_rvl_nodes_configure_packet_parses);
  return UNITY_END();
}
