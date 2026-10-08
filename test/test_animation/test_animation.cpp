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
#include <initializer_list>
#include <optional>
#include <rvl.hpp>
#include <rvl/config.hpp>
#include <rvl/protocols/network_state.hpp>
#include <unity.h>
#include <variant>

#define LOCAL_ID 10
#define CONTROLLER_ID 20

FakeAnimation& animation = fake.animationEndpoint;

uint32_t updates = 0;

void countUpdate() {
  updates++;
}

// The wire layout of the settings: time period, distance period, then each
// layer's h, s, v and a, each as a, b, w_t, w_x, phi
Bytes parametricPayload(const RVLParametricSettings& settings) {
  PacketWriter payload;
  payload.u8(settings.timePeriod).u8(settings.distancePeriod);
  for (auto& layer : settings.layers) {
    for (auto* component : {&layer.h, &layer.s, &layer.v, &layer.a}) {
      payload.u8(component->a)
          .u8(component->b)
          .u8(component->w_t)
          .u8(component->w_x)
          .u8(component->phi);
    }
  }
  return payload.bytes;
}

// The scene prefix every RVLA payload starts with, then the content
Bytes scenePayload(uint32_t start, uint8_t fade, const Bytes& content = {}) {
  PacketWriter payload;
  payload.u32(start).u8(0).u8(fade);
  payload.bytes.insert(payload.bytes.end(), content.begin(), content.end());
  return payload.bytes;
}

// An off scene due now. With no controller adopted yet, the first source an
// RVLA packet reaches NetworkState from becomes the controller, so off shows
// whether a packet got past the container
Bytes off(uint8_t source = CONTROLLER_ID, uint8_t channel = 0) {
  return rvlaPacket(source, PACKET_TYPE_OFF, channel,
      scenePayload(rvl::getAnimationFrame(), MIN_FADE_FRAMES));
}

Bytes parametric(const RVLParametricSettings& settings, uint32_t start,
    uint8_t fade = MIN_FADE_FRAMES) {
  return rvlaPacket(CONTROLLER_ID, PACKET_TYPE_PARAMETRIC_ANIMATION, 0,
      scenePayload(start, fade, parametricPayload(settings)));
}

// Every field different, and the signed ones negative
RVLParametricSettings distinctiveParametric() {
  RVLParametricSettings settings;
  settings.timePeriod = 200;
  settings.distancePeriod = 16;
  uint8_t n = 0;
  for (auto& layer : settings.layers) {
    for (auto* component : {&layer.h, &layer.s, &layer.v, &layer.a}) {
      component->a = 100 + n;
      component->b = 200 + n;
      component->w_t = -1 - n;
      component->w_x = 1 + n;
      component->phi = -64 + n;
      n++;
    }
  }
  return settings;
}

// Distinctive settings told apart by one hue
RVLParametricSettings parametricVariant(uint8_t id) {
  RVLParametricSettings settings = distinctiveParametric();
  settings.layers[0].h.b = id;
  return settings;
}

void loopAt(uint32_t time) {
  fake.clock = time;
  rvl::loop();
}

// The local time frame f begins at
uint32_t frameStart(uint32_t f) {
  return f * FRAME_PERIOD - rvl::toAnimationClock(0);
}

void loopAtFrame(uint32_t f) {
  loopAt(frameStart(f));
}

// One iteration a frame, through f
void loopFramesThrough(uint32_t f) {
  for (uint32_t next = rvl::getAnimationFrame() + 1;
      rvl::subtractFrames(f, next) >= 0; next++)
  {
    loopAtFrame(next);
  }
}

// A scene scheduled now has started and finished fading
void settle() {
  loopAt(fake.clock + (SCENE_LEAD_FRAMES + 1) * FRAME_PERIOD);
  loopAt(fake.clock + (UINT8_MAX + 1) * FRAME_PERIOD);
}

void receiveNow(const Bytes& packet) {
  animation.receive(packet);
  rvl::loop();
}

// A received scene becomes current once its start and fade have passed
void deliver(const Bytes& packet) {
  receiveNow(packet);
  settle();
}

// Current becomes these settings, with no fade running to hold the next scene
void show(const RVLParametricSettings& settings) {
  rvl::scheduleScene({rvl::getAnimationFrame(), MIN_FADE_FRAMES, settings});
  settle();
}

void syncClock() {
  rvl::NetworkState::refreshLocalClockSynchronization();
}

bool isOff() {
  return std::holds_alternative<RVLOff>(rvl::getCurrentScene().content);
}

bool isShowing(const RVLParametricSettings& settings) {
  RVLScene current = rvl::getCurrentScene();
  auto* shown = std::get_if<RVLParametricSettings>(&current.content);
  return shown != nullptr && *shown == settings;
}

uint8_t packetType(const SentPacket& packet) {
  return packet.bytes[6];
}

// The start frame, at the head of the scene prefix
uint32_t sceneStart(const SentPacket& packet) {
  return readU32(packet.bytes, 9);
}

// A controller's packet fed back to a receiver needs a peer's source, or the
// receiver drops it as its own
Bytes fromController(SentPacket packet) {
  packet.bytes[5] = CONTROLLER_ID;
  return packet.bytes;
}

void setUp() {
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  rvl::setLinkUpState(true);
  rvl::setDeviceId(LOCAL_ID);
  // A channel change forgets the controller
  rvl::setChannel(1);
  rvl::setChannel(0);
  rvl::Scenes::reset();
  // The boot scene is off, and these tests tell a packet got through by off
  RVLParametricSettings defaults;
  show(defaults);
  animation.sent.clear();
  fake.output.clear();
  updates = 0;
}

// Every path through the dispatcher ends the read, and a well-formed packet is
// never read past its end
void tearDown() {
  TEST_ASSERT_EQUAL(0, animation.unendedReads);
  TEST_ASSERT_EQUAL(0, animation.readsPastEnd);
}

// Must run first: nothing has synced the clock yet, and nothing un-syncs it
void test_a_packet_before_the_first_sync_is_dropped_and_adopts_no_controller() {
  RVLParametricSettings settings = distinctiveParametric();
  deliver(parametric(settings, rvl::getAnimationFrame()));
  TEST_ASSERT_FALSE(isShowing(settings));
  TEST_ASSERT_FALSE(rvl::NetworkState::isControllerActive());
}

// Must run before the first sync too
void test_a_controller_sends_nothing_before_its_first_sync() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  uint32_t start = fake.clock;
  rvl::setOff();
  for (uint32_t elapsed = 1000; elapsed <= 4000; elapsed += 1000) {
    loopAt(start + elapsed);
  }
  TEST_ASSERT_EQUAL(0, animation.sent.size());
}

// Syncs the clock, which every test after it relies on
void test_the_same_packet_is_accepted_after_the_first_sync() {
  syncClock();
  RVLParametricSettings settings = distinctiveParametric();
  deliver(parametric(settings, rvl::getAnimationFrame()));
  TEST_ASSERT_TRUE(isShowing(settings));
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
  packet[4] = RVLA_VERSION + 1;
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
  RVLParametricSettings defaults;
  show(defaults);
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

// Only the payload can be the wrong length: the header is read before the type
// is known
void test_a_packet_of_the_wrong_payload_length_is_dropped_and_logged() {
  RVLParametricSettings settings = distinctiveParametric();
  uint32_t n = rvl::getAnimationFrame();
  Bytes shortContent = parametricPayload(settings);
  shortContent.pop_back();
  Bytes longContent = parametricPayload(settings);
  longContent.push_back(0);
  const Bytes packets[] = {
      rvlaPacket(CONTROLLER_ID, PACKET_TYPE_OFF, 0,
          PacketWriter().u32(n).u8(MIN_FADE_FRAMES).bytes),
      rvlaPacket(CONTROLLER_ID, PACKET_TYPE_OFF, 0,
          scenePayload(n, MIN_FADE_FRAMES, {0})),
      rvlaPacket(CONTROLLER_ID, PACKET_TYPE_PARAMETRIC_ANIMATION, 0,
          scenePayload(n, MIN_FADE_FRAMES, shortContent)),
      rvlaPacket(CONTROLLER_ID, PACKET_TYPE_PARAMETRIC_ANIMATION, 0,
          scenePayload(n, MIN_FADE_FRAMES, longContent)),
  };
  for (const Bytes& packet : packets) {
    fake.output.clear();
    deliver(packet);
    TEST_ASSERT_FALSE(isOff());
    TEST_ASSERT_FALSE(isShowing(settings));
    TEST_ASSERT_TRUE(fake.logged("bytes, expected"));
  }
}

void test_a_sent_parametric_round_trips() {
  RVLParametricSettings settings = distinctiveParametric();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  uint32_t n = rvl::getAnimationFrame();
  rvl::setParametricSettings(&settings);
  // Current, then the scene just scheduled: the lead ahead, the default fade
  TEST_ASSERT_EQUAL(2, animation.sent.size());
  TEST_ASSERT(animation.sent[1].destination == Destination::Channel);
  TEST_ASSERT_PACKET(rvlaPacket(LOCAL_ID, PACKET_TYPE_PARAMETRIC_ANIMATION, 0,
                         scenePayload(n + SCENE_LEAD_FRAMES,
                             DEFAULT_FADE_FRAMES, parametricPayload(settings))),
      animation.sent[1].bytes);

  Bytes packet = fromController(animation.sent[1]);
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  deliver(packet);
  RVLScene expected = {n + SCENE_LEAD_FRAMES, DEFAULT_FADE_FRAMES, settings};
  TEST_ASSERT_TRUE(rvl::getCurrentScene() == expected);
}

void test_a_sent_off_round_trips_and_a_parametric_after_it_restores_it() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  uint32_t n = rvl::getAnimationFrame();
  rvl::setOff();
  TEST_ASSERT_EQUAL(2, animation.sent.size());
  TEST_ASSERT_PACKET(
      rvlaPacket(LOCAL_ID, PACKET_TYPE_OFF, 0,
          scenePayload(n + SCENE_LEAD_FRAMES, DEFAULT_FADE_FRAMES)),
      animation.sent[1].bytes);

  Bytes packet = fromController(animation.sent[1]);
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  deliver(packet);
  TEST_ASSERT_TRUE(isOff());
  RVLScene expected = {n + SCENE_LEAD_FRAMES, DEFAULT_FADE_FRAMES, RVLOff{}};
  TEST_ASSERT_TRUE(rvl::getCurrentScene() == expected);

  RVLParametricSettings settings = distinctiveParametric();
  deliver(parametric(settings, rvl::getAnimationFrame()));
  TEST_ASSERT_FALSE(isOff());
  TEST_ASSERT_TRUE(isShowing(settings));
}

void test_a_resent_scene_is_ignored() {
  RVLParametricSettings settings = distinctiveParametric();
  Bytes packet = parametric(settings, rvl::getAnimationFrame());
  deliver(packet);
  TEST_ASSERT_TRUE(isShowing(settings));
  uint32_t accepted = updates;
  deliver(packet);
  TEST_ASSERT_EQUAL(accepted, updates);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
}

void test_a_fade_below_the_floor_is_floored() {
  RVLParametricSettings settings = distinctiveParametric();
  receiveNow(parametric(settings, rvl::getAnimationFrame() + 100, 3));
  std::optional<RVLScene> pending = rvl::getPendingScene();
  TEST_ASSERT_TRUE(pending.has_value());
  TEST_ASSERT_EQUAL(MIN_FADE_FRAMES, pending->fade);
}

// The writer's zero is checked by the round trips, byte for byte
void test_the_reserved_byte_is_ignored_on_read() {
  RVLParametricSettings settings = distinctiveParametric();
  uint32_t start = rvl::getAnimationFrame() + 100;
  Bytes packet = parametric(settings, start, 40);
  packet[13] = 0xAB;
  receiveNow(packet);
  std::optional<RVLScene> pending = rvl::getPendingScene();
  TEST_ASSERT_TRUE(pending.has_value());
  RVLScene expected = {start, 40, settings};
  TEST_ASSERT_TRUE(*pending == expected);
}

// The renderer divides by both periods, so a zero would panic the board
void test_a_parametric_with_a_zero_period_is_dropped_and_logged() {
  RVLParametricSettings defaults;
  RVLParametricSettings settings = distinctiveParametric();
  settings.timePeriod = 0;
  deliver(parametric(settings, rvl::getAnimationFrame()));
  TEST_ASSERT_TRUE(isShowing(defaults));
  TEST_ASSERT_TRUE(fake.logged("zero period"));

  settings = distinctiveParametric();
  settings.distancePeriod = 0;
  deliver(parametric(settings, rvl::getAnimationFrame()));
  TEST_ASSERT_TRUE(isShowing(defaults));
}

// The repeat is the periodic path pulled forward, so it carries current then
// pending; once pending has activated the interval carries current alone
void test_the_periodic_sender_repeats_current_then_pending() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  uint32_t n = rvl::getAnimationFrame();
  rvl::setOff();
  TEST_ASSERT_EQUAL(2, animation.sent.size());
  TEST_ASSERT_EQUAL(
      PACKET_TYPE_PARAMETRIC_ANIMATION, packetType(animation.sent[0]));
  TEST_ASSERT_EQUAL(PACKET_TYPE_OFF, packetType(animation.sent[1]));
  animation.sent.clear();

  uint32_t repeatAt = frameStart(n + REPEAT_SEND_FRAMES);
  loopAt(repeatAt - 1);
  TEST_ASSERT_EQUAL(0, animation.sent.size());
  loopAt(repeatAt);
  TEST_ASSERT_EQUAL(2, animation.sent.size());
  TEST_ASSERT_EQUAL(
      PACKET_TYPE_PARAMETRIC_ANIMATION, packetType(animation.sent[0]));
  TEST_ASSERT_EQUAL(PACKET_TYPE_OFF, packetType(animation.sent[1]));
  animation.sent.clear();

  loopAt(repeatAt + CLIENT_SYNC_INTERVAL - 1);
  TEST_ASSERT_EQUAL(0, animation.sent.size());
  TEST_ASSERT_TRUE(isOff());
  loopAt(repeatAt + CLIENT_SYNC_INTERVAL);
  TEST_ASSERT_EQUAL(1, animation.sent.size());
  TEST_ASSERT_EQUAL(PACKET_TYPE_OFF, packetType(animation.sent[0]));
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

// The gate from the wire side: a change sends at once and on the boundary
// four frames on, changes during the dissolve send nothing, and the last of
// them goes out when the dissolve ends, starting the lead after that frame
void test_changes_during_a_dissolve_are_held_until_it_ends() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  uint32_t n = rvl::getAnimationFrame() + 1;
  loopAtFrame(n);
  RVLParametricSettings first = parametricVariant(1);
  rvl::setParametricSettings(&first);
  TEST_ASSERT_EQUAL(2, animation.sent.size());
  TEST_ASSERT_EQUAL_UINT32(
      n + SCENE_LEAD_FRAMES, sceneStart(animation.sent[1]));
  animation.sent.clear();

  uint32_t fadeEnd = n + SCENE_LEAD_FRAMES + DEFAULT_FADE_FRAMES;
  for (uint32_t f = n + 1; f < fadeEnd; f++) {
    loopAtFrame(f);
    if (f == n + REPEAT_SEND_FRAMES) {
      TEST_ASSERT_EQUAL(2, animation.sent.size());
      animation.sent.clear();
    }
    if (f >= n + 6 && f < n + 16) {
      RVLParametricSettings change = parametricVariant(f - n);
      rvl::setParametricSettings(&change);
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, animation.sent.size(), "sent mid-dissolve");
  }

  loopAtFrame(fadeEnd);
  TEST_ASSERT_EQUAL(2, animation.sent.size());
  TEST_ASSERT_EQUAL_UINT32(
      n + SCENE_LEAD_FRAMES, sceneStart(animation.sent[0]));
  TEST_ASSERT_PACKET(
      rvlaPacket(LOCAL_ID, PACKET_TYPE_PARAMETRIC_ANIMATION, 0,
          scenePayload(fadeEnd + SCENE_LEAD_FRAMES, DEFAULT_FADE_FRAMES,
              parametricPayload(parametricVariant(15)))),
      animation.sent[1].bytes);
  animation.sent.clear();
  loopFramesThrough(fadeEnd + REPEAT_SEND_FRAMES - 1);
  TEST_ASSERT_EQUAL(0, animation.sent.size());
  loopAtFrame(fadeEnd + REPEAT_SEND_FRAMES);
  TEST_ASSERT_EQUAL(2, animation.sent.size());
  animation.sent.clear();

  // A quiet spell after the released scene's own dissolve, short of the
  // periodic send, then a change sends at once again
  loopFramesThrough(fadeEnd + SCENE_LEAD_FRAMES + DEFAULT_FADE_FRAMES + 10);
  TEST_ASSERT_EQUAL(0, animation.sent.size());
  RVLParametricSettings later = parametricVariant(99);
  rvl::setParametricSettings(&later);
  TEST_ASSERT_EQUAL(2, animation.sent.size());
}

// The repeat's copies reach most receivers after the pending scene has
// activated, where the old current is only previous
void test_the_repeats_packets_change_nothing_on_a_receiver_that_has_them() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  RVLParametricSettings a = parametricVariant(1);
  RVLParametricSettings b = parametricVariant(2);
  show(a);
  uint32_t n = rvl::getAnimationFrame() + 1;
  loopAtFrame(n);
  animation.sent.clear();
  rvl::setParametricSettings(&b);
  TEST_ASSERT_EQUAL(2, animation.sent.size());
  Bytes currentPacket = fromController(animation.sent[0]);
  Bytes pendingPacket = fromController(animation.sent[1]);

  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  animation.receive(currentPacket);
  animation.receive(pendingPacket);
  rvl::loop();
  TEST_ASSERT_TRUE(isShowing(a));
  TEST_ASSERT_TRUE(rvl::getPendingScene().has_value());
  loopFramesThrough(n + 30);
  TEST_ASSERT_TRUE(isShowing(b));
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());

  uint32_t accepted = updates;
  animation.receive(currentPacket);
  animation.receive(pendingPacket);
  rvl::loop();
  TEST_ASSERT_EQUAL(accepted, updates);
  TEST_ASSERT_TRUE(isShowing(b));
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
}

// The packets rvl-node's wire test asserts its sender writes, on channel 3,
// read while their starts are still ahead so they pend rather than activate
void test_rvl_nodes_off_packet_parses() {
  rvl::setChannel(3);
  loopAtFrame(0x00FEDCBA - 10);
  receiveNow(GOLDEN_OFF_PACKET);
  std::optional<RVLScene> pending = rvl::getPendingScene();
  TEST_ASSERT_TRUE(pending.has_value());
  TEST_ASSERT_EQUAL_UINT32(0x00FEDCBA, pending->start);
  TEST_ASSERT_EQUAL(255, pending->fade);
  TEST_ASSERT_TRUE(std::holds_alternative<RVLOff>(pending->content));
}

void test_rvl_nodes_parametric_packet_parses() {
  rvl::setChannel(3);
  loopAtFrame(0x00ABCDEF - 10);
  receiveNow(GOLDEN_PARAMETRIC_PACKET);
  std::optional<RVLScene> pending = rvl::getPendingScene();
  TEST_ASSERT_TRUE(pending.has_value());
  TEST_ASSERT_EQUAL_UINT32(0x00ABCDEF, pending->start);
  TEST_ASSERT_EQUAL(40, pending->fade);
  auto* settings = std::get_if<RVLParametricSettings>(&pending->content);
  TEST_ASSERT_NOT_NULL(settings);
  TEST_ASSERT_TRUE(*settings == distinctiveParametric());
}

int main() {
  rvl::init(&fake);
  rvl::on(EVENT_ANIMATION_UPDATED, countUpdate);
  UNITY_BEGIN();
  RUN_TEST(
      test_a_packet_before_the_first_sync_is_dropped_and_adopts_no_controller);
  RUN_TEST(test_a_controller_sends_nothing_before_its_first_sync);
  RUN_TEST(test_the_same_packet_is_accepted_after_the_first_sync);
  RUN_TEST(test_off_from_a_controller_on_the_channel_selects_off);
  RUN_TEST(test_a_bad_signature_is_dropped);
  RUN_TEST(test_another_version_is_dropped_and_logged);
  RUN_TEST(test_the_nodes_own_packets_are_dropped);
  RUN_TEST(test_sources_of_240_and_up_are_dropped);
  RUN_TEST(test_only_the_nodes_channel_is_accepted);
  RUN_TEST(test_everything_is_discarded_while_the_node_has_no_id);
  RUN_TEST(test_an_unknown_packet_type_is_logged);
  RUN_TEST(test_a_packet_of_the_wrong_payload_length_is_dropped_and_logged);
  RUN_TEST(test_a_sent_parametric_round_trips);
  RUN_TEST(test_a_sent_off_round_trips_and_a_parametric_after_it_restores_it);
  RUN_TEST(test_a_resent_scene_is_ignored);
  RUN_TEST(test_a_fade_below_the_floor_is_floored);
  RUN_TEST(test_the_reserved_byte_is_ignored_on_read);
  RUN_TEST(test_a_parametric_with_a_zero_period_is_dropped_and_logged);
  RUN_TEST(test_the_periodic_sender_repeats_current_then_pending);
  RUN_TEST(test_a_controller_without_an_id_sends_nothing);
  RUN_TEST(test_a_receiver_never_sends);
  RUN_TEST(test_changes_during_a_dissolve_are_held_until_it_ends);
  RUN_TEST(test_the_repeats_packets_change_nothing_on_a_receiver_that_has_them);
  RUN_TEST(test_rvl_nodes_off_packet_parses);
  RUN_TEST(test_rvl_nodes_parametric_packet_parses);
  return UNITY_END();
}
