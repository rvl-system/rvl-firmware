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
#include <rvl.hpp>
#include <rvl/config.hpp>
#include <rvl/protocols/network_state.hpp>
#include <unity.h>
#include <vector>

#define LOCAL_ID 10

const uint32_t LAST_FRAME = (1u << 27) - 1;

uint32_t updates = 0;

void countUpdate() {
  updates++;
}

// Contents told apart by one hue, which readId() reads back; off reads as -1
RVLParametricSettings makeSettings(uint8_t id) {
  RVLParametricSettings settings;
  settings.layers[0].h.b = id;
  return settings;
}

int readId(const RVLSceneContent& content) {
  auto* settings = std::get_if<RVLParametricSettings>(&content);
  return settings ? settings->layers[0].h.b : -1;
}

int readId(const RVLScene& scene) {
  return readId(scene.content);
}

RVLScene makeScene(
    uint32_t start, uint8_t fade, const RVLSceneContent& content) {
  return {start, fade, content};
}

// A controller board's change, as its UI makes one
void requestContent(uint8_t id) {
  RVLParametricSettings requested = makeSettings(id);
  rvl::setParametricSettings(&requested);
}

RVLScene readPendingScene() {
  std::optional<RVLScene> scene = rvl::getPendingScene();
  TEST_ASSERT_TRUE_MESSAGE(scene.has_value(), "a pending scene");
  return *scene;
}

// ms into frame f of the animation clock
void setFrame(uint32_t f, uint32_t ms = 0) {
  fake.clock = f * FRAME_PERIOD + ms - rvl::toAnimationClock(0);
}

// One background iteration, ms into frame f
void loopAt(uint32_t f, uint32_t ms = 0) {
  setFrame(f, ms);
  rvl::loop();
}

// One iteration a frame through f, as the aligned background loop runs
void runThrough(uint32_t f) {
  uint32_t next = rvl::getAnimationFrame() + 1;
  for (; rvl::subtractFrames(f, next) >= 0; next++) {
    loopAt(next);
  }
}

// Forward to frame f in hops under 2^31 ms, looping at each, so no millisecond
// timer reads the new time as the past
void hopTo(uint32_t f) {
  uint32_t target = f * FRAME_PERIOD - rvl::toAnimationClock(0);
  while (target - fake.clock >= (1u << 30)) {
    fake.clock += 1u << 30;
    rvl::loop();
  }
  loopAt(f);
}

rvl::RenderPlan readPlanAt(uint32_t f, uint32_t ms = 0) {
  setFrame(f, ms);
  return rvl::getRenderPlan();
}

// Each test starts well clear of the last one's frames
uint32_t moveToFreshFrame() {
  uint32_t f = rvl::getAnimationFrame() + 1000;
  hopTo(f);
  return f;
}

void setUp() {
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  rvl::setChannel(0);
  rvl::Scenes::reset();
  fake.animationEndpoint.sent.clear();
  fake.output.clear();
  updates = 0;
}

void tearDown() {}

void test_a_future_scene_pends_and_activates_at_its_start() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n + 5, 32, makeSettings(1)));
  TEST_ASSERT_EQUAL(1, readId(readPendingScene()));
  loopAt(n + 4, 31);
  TEST_ASSERT_EQUAL(-1, readId(rvl::getCurrentScene()));
  loopAt(n + 5);
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
}

void test_the_amount_runs_over_the_fade_by_frame() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n + 5, 32, makeSettings(1)));

  // A render ahead of this frame's activation shows the old scene, as the
  // activation's amount of 0 does
  auto early = readPlanAt(n + 5);
  TEST_ASSERT_FALSE(early.fading);
  TEST_ASSERT_EQUAL(-1, readId(early.current));
  loopAt(n + 5);
  auto first = readPlanAt(n + 5, 31);
  TEST_ASSERT_TRUE(first.fading);
  TEST_ASSERT_EQUAL(0, first.amount);
  TEST_ASSERT_EQUAL(-1, readId(first.previous));
  TEST_ASSERT_EQUAL(1, readId(first.current));

  uint8_t last = 0;
  for (uint32_t k = 0; k < 32; k++) {
    auto plan = readPlanAt(n + 5 + k);
    TEST_ASSERT_TRUE(plan.fading);
    TEST_ASSERT_EQUAL(k * 255 / 32, plan.amount);
    TEST_ASSERT_EQUAL(plan.amount, readPlanAt(n + 5 + k, 31).amount);
    TEST_ASSERT_GREATER_OR_EQUAL(last, plan.amount);
    last = plan.amount;
  }
  auto done = readPlanAt(n + 37);
  TEST_ASSERT_FALSE(done.fading);
  TEST_ASSERT_EQUAL(0, done.amount);
}

void test_a_late_board_fades_over_the_remainder_and_ends_with_the_fleet() {
  uint32_t n = moveToFreshFrame();
  loopAt(n + 10);
  rvl::scheduleScene(makeScene(n, 64, makeSettings(1)));
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
  TEST_ASSERT_EQUAL(0, readPlanAt(n + 10).amount);
  TEST_ASSERT_EQUAL(27 * 255 / 54, readPlanAt(n + 37).amount);
  TEST_ASSERT_TRUE(readPlanAt(n + 63).fading);
  TEST_ASSERT_FALSE(readPlanAt(n + 64).fading);
}

void test_a_board_later_than_the_remainder_allows_fades_over_the_floor() {
  uint32_t n = moveToFreshFrame();
  loopAt(n + 20);
  rvl::scheduleScene(makeScene(n, 32, makeSettings(1)));
  TEST_ASSERT_TRUE(readPlanAt(n + 20 + MIN_FADE_FRAMES - 1).fading);
  TEST_ASSERT_FALSE(readPlanAt(n + 20 + MIN_FADE_FRAMES).fading);

  // Past its whole fade
  loopAt(n + 200);
  rvl::scheduleScene(makeScene(n + 100, 32, makeSettings(2)));
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));
  TEST_ASSERT_TRUE(readPlanAt(n + 200 + MIN_FADE_FRAMES - 1).fading);
  TEST_ASSERT_FALSE(readPlanAt(n + 200 + MIN_FADE_FRAMES).fading);
}

void test_a_scene_nearly_half_a_cycle_late_fades_over_the_floor() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n - ((1u << 26) - 1), 255, makeSettings(1)));
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
  TEST_ASSERT_TRUE(readPlanAt(n + MIN_FADE_FRAMES - 1).fading);
  TEST_ASSERT_FALSE(readPlanAt(n + MIN_FADE_FRAMES).fading);
}

void test_a_scene_the_node_holds_is_a_resend() {
  uint32_t n = moveToFreshFrame();
  RVLScene a = makeScene(n + 5, 16, makeSettings(1));
  rvl::scheduleScene(a);
  rvl::scheduleScene(a);
  TEST_ASSERT_EQUAL(1, updates);
  runThrough(n + 5);
  rvl::scheduleScene(a);
  TEST_ASSERT_EQUAL(1, updates);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
}

// A controller's clock step can re-schedule new content at a start receivers
// already hold, whether pending or activated
void test_a_same_start_with_other_content_is_a_new_scene() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n + 5, 16, makeSettings(1)));
  rvl::scheduleScene(makeScene(n + 5, 16, makeSettings(2)));
  TEST_ASSERT_EQUAL(2, readId(readPendingScene()));
  runThrough(n + 5);
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));
  rvl::scheduleScene(makeScene(n + 5, 16, makeSettings(3)));
  TEST_ASSERT_EQUAL(3, readId(readPendingScene()));
  runThrough(n + 21);
  TEST_ASSERT_EQUAL(3, readId(rvl::getCurrentScene()));
}

void test_a_due_scene_displaces_any_pending_and_a_future_one_only_a_future_one() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 32, makeSettings(1)));
  rvl::scheduleScene(makeScene(n + 100, 16, makeSettings(2)));
  rvl::scheduleScene(makeScene(n + 50, 16, makeSettings(3)));
  TEST_ASSERT_EQUAL(3, readId(readPendingScene()));
  rvl::scheduleScene(makeScene(n, 16, makeSettings(4)));
  TEST_ASSERT_EQUAL(4, readId(readPendingScene()));
  uint32_t before = updates;
  rvl::scheduleScene(makeScene(n + 50, 16, makeSettings(5)));
  TEST_ASSERT_EQUAL(4, readId(readPendingScene()));
  TEST_ASSERT_EQUAL(before, updates);
}

void test_a_channel_toggle_accepts_a_resend_of_the_scene_it_left() {
  uint32_t n = moveToFreshFrame();
  RVLScene s = makeScene(n, 16, makeSettings(1));
  rvl::scheduleScene(s);
  runThrough(n + 20);
  rvl::setChannel(1);
  rvl::setChannel(0);
  TEST_ASSERT_EQUAL(-1, readId(rvl::getCurrentScene()));
  rvl::scheduleScene(s);
  auto plan = rvl::getRenderPlan();
  TEST_ASSERT_EQUAL(1, readId(plan.current));
  TEST_ASSERT_EQUAL(-1, readId(plan.previous));
  TEST_ASSERT_TRUE(plan.fading);
}

// The sender's repeat and periodic sends still carry the scene a receiver has
// just left, and arrive after it activated the next one
void test_the_senders_copy_of_the_scene_just_left_changes_nothing() {
  uint32_t n = moveToFreshFrame();
  RVLScene a = makeScene(n, 16, makeSettings(1));
  RVLScene b = makeScene(n + 30, 16, makeSettings(2));
  rvl::scheduleScene(a);
  rvl::scheduleScene(b);
  runThrough(n + 30);
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));

  loopAt(n + 31);
  rvl::scheduleScene(a);
  rvl::scheduleScene(b);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  loopAt(n + 60);
  rvl::scheduleScene(a);
  rvl::scheduleScene(b);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  runThrough(n + 100);
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));
}

void test_a_due_scene_waits_out_a_running_fade() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 32, makeSettings(1)));
  loopAt(n + 10);
  uint8_t amount = rvl::getRenderPlan().amount;
  rvl::scheduleScene(makeScene(n + 10, 16, makeSettings(2)));
  auto plan = rvl::getRenderPlan();
  TEST_ASSERT_EQUAL(1, readId(plan.current));
  TEST_ASSERT_EQUAL(amount, plan.amount);
  runThrough(n + 31);
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
  loopAt(n + 32);
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));
  // Its own fade ended at n + 26, so it catches up over the floor
  TEST_ASSERT_TRUE(readPlanAt(n + 32 + MIN_FADE_FRAMES - 1).fading);
  TEST_ASSERT_FALSE(readPlanAt(n + 32 + MIN_FADE_FRAMES).fading);
}

// The drain runs before the loop, so a due scene can arrive in the iteration a
// fade ends, while an older due scene still waits on that fade
void test_a_due_scene_arriving_as_a_fade_ends_supersedes_a_waiting_one() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 32, makeSettings(1)));
  rvl::scheduleScene(makeScene(n + 10, 16, makeSettings(2)));
  setFrame(n + 32);
  rvl::scheduleScene(makeScene(n + 20, 16, makeSettings(3)));
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  runThrough(n + 100);
  TEST_ASSERT_EQUAL(3, readId(rvl::getCurrentScene()));
}

void test_a_scene_starting_inside_a_fade_fades_over_what_remains() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 32, makeSettings(1)));
  rvl::scheduleScene(makeScene(n + 20, 32, makeSettings(2)));
  runThrough(n + 32);
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));
  TEST_ASSERT_TRUE(readPlanAt(n + 51).fading);
  TEST_ASSERT_FALSE(readPlanAt(n + 52).fading);
}

void test_a_senders_current_and_pending_in_one_drain_both_land() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n - 20, 16, makeSettings(1)));
  rvl::scheduleScene(makeScene(n + 30, 16, makeSettings(2)));
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
  TEST_ASSERT_EQUAL(2, readId(readPendingScene()));
}

// What the fleet shows now outranks what it shows next, which the next re-send
// brings back
void test_mid_fade_the_due_scene_of_a_pair_outranks_the_future_one() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 32, makeSettings(1)));
  loopAt(n + 5);
  rvl::scheduleScene(makeScene(n - 20, 16, makeSettings(2)));
  rvl::scheduleScene(makeScene(n + 40, 16, makeSettings(3)));
  TEST_ASSERT_EQUAL(2, readId(readPendingScene()));
  runThrough(n + 32);
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
}

void test_the_next_scene_arriving_before_the_current_one_still_pends() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n + 30, 16, makeSettings(2)));
  rvl::scheduleScene(makeScene(n - 2, 16, makeSettings(1)));
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
  TEST_ASSERT_EQUAL(2, readId(readPendingScene()));
}

void test_a_scene_with_the_current_content_has_no_fade() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 16, makeSettings(1)));
  runThrough(n + 20);
  rvl::scheduleScene(makeScene(n + 20, 16, makeSettings(1)));
  TEST_ASSERT_EQUAL_UINT32(n + 20, rvl::getCurrentScene().start);
  TEST_ASSERT_FALSE(rvl::getRenderPlan().fading);
  rvl::scheduleScene(makeScene(n + 20, 16, makeSettings(2)));
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));
}

void test_off_fades_in_both_directions() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 16, makeSettings(1)));
  auto in = rvl::getRenderPlan();
  TEST_ASSERT_TRUE(in.fading);
  TEST_ASSERT_EQUAL(-1, readId(in.previous));
  TEST_ASSERT_EQUAL(1, readId(in.current));
  runThrough(n + 20);
  rvl::scheduleScene(makeScene(n + 20, 16, RVLOff{}));
  auto out = rvl::getRenderPlan();
  TEST_ASSERT_TRUE(out.fading);
  TEST_ASSERT_EQUAL(1, readId(out.previous));
  TEST_ASSERT_EQUAL(-1, readId(out.current));
}

void test_no_fade_is_reported_at_boot() {
  uint32_t n = moveToFreshFrame();
  auto plan = readPlanAt(n);
  TEST_ASSERT_FALSE(plan.fading);
  TEST_ASSERT_EQUAL(0, plan.amount);
  TEST_ASSERT_EQUAL(-1, readId(plan.current));
}

void test_the_update_event_fires_on_acceptance_not_activation() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n + 5, 16, makeSettings(1)));
  TEST_ASSERT_EQUAL(1, updates);
  runThrough(n + 30);
  TEST_ASSERT_EQUAL(1, updates);
}

void test_a_controllers_change_starts_a_lead_from_now() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  RVLScene p = readPendingScene();
  TEST_ASSERT_EQUAL_UINT32(n + SCENE_LEAD_FRAMES, p.start);
  TEST_ASSERT_EQUAL(DEFAULT_FADE_FRAMES, p.fade);
  TEST_ASSERT_EQUAL(1, updates);
  // Its own render waits for the start, as every receiver's does
  runThrough(n + SCENE_LEAD_FRAMES - 1);
  TEST_ASSERT_EQUAL(-1, readId(rvl::getCurrentScene()));
  loopAt(n + SCENE_LEAD_FRAMES);
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
}

void test_changes_during_the_lead_and_fade_are_held_and_the_last_wins() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  loopAt(n + 2);
  requestContent(2);
  requestContent(3);
  TEST_ASSERT_EQUAL(1, updates);
  TEST_ASSERT_EQUAL(1, readId(readPendingScene()));

  uint32_t fadeEnd = n + SCENE_LEAD_FRAMES + DEFAULT_FADE_FRAMES;
  runThrough(fadeEnd - 1);
  TEST_ASSERT_EQUAL(1, updates);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  loopAt(fadeEnd);
  TEST_ASSERT_EQUAL(2, updates);
  RVLScene p = readPendingScene();
  TEST_ASSERT_EQUAL(3, readId(p));
  TEST_ASSERT_EQUAL_UINT32(fadeEnd + SCENE_LEAD_FRAMES, p.start);
  runThrough(fadeEnd + SCENE_LEAD_FRAMES);
  TEST_ASSERT_EQUAL(3, readId(rvl::getCurrentScene()));
  TEST_ASSERT_TRUE(rvl::getRenderPlan().fading);
}

// A flash write can hold the background loop past the frame a fade ends
void test_the_gate_opens_in_a_late_iteration_too() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  requestContent(2);
  loopAt(n + SCENE_LEAD_FRAMES);
  uint32_t late = n + SCENE_LEAD_FRAMES + DEFAULT_FADE_FRAMES + 7;
  loopAt(late);
  TEST_ASSERT_EQUAL_UINT32(late + SCENE_LEAD_FRAMES, readPendingScene().start);
}

void test_off_on_an_off_strip_and_a_dial_turned_back_do_nothing() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::setOff();
  TEST_ASSERT_EQUAL(0, updates);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  requestContent(1);
  runThrough(n + 30);
  requestContent(1);
  TEST_ASSERT_EQUAL(1, updates);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
}

void test_a_request_back_to_the_pending_scene_clears_the_held_one() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  requestContent(2);
  requestContent(1);
  runThrough(n + 60);
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
  TEST_ASSERT_EQUAL(1, updates);
}

void test_a_request_back_to_the_fading_scene_clears_the_held_one() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  runThrough(n + SCENE_LEAD_FRAMES + 2);
  requestContent(2);
  requestContent(1);
  runThrough(n + 60);
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
  TEST_ASSERT_EQUAL(1, updates);
}

void test_repeating_the_held_request_holds_it() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  requestContent(2);
  requestContent(2);
  runThrough(n + 60);
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));
  TEST_ASSERT_EQUAL(2, updates);
}

void test_a_receivers_channel_switch_drops_its_pending_scene() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 16, makeSettings(1)));
  rvl::scheduleScene(makeScene(n + 30, 16, makeSettings(2)));
  rvl::setChannel(1);
  runThrough(n + 40);
  TEST_ASSERT_EQUAL(-1, readId(rvl::getCurrentScene()));
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  rvl::scheduleScene(makeScene(n + 40, 16, makeSettings(3)));
  auto plan = rvl::getRenderPlan();
  TEST_ASSERT_EQUAL(3, readId(plan.current));
  TEST_ASSERT_EQUAL(-1, readId(plan.previous));
  TEST_ASSERT_TRUE(plan.fading);
}

void test_switching_to_receiver_returns_to_the_boot_scene() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  runThrough(n + 30);
  requestContent(2);
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  TEST_ASSERT_EQUAL(-1, readId(rvl::getCurrentScene()));
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  runThrough(n + 60);
  TEST_ASSERT_EQUAL(-1, readId(rvl::getCurrentScene()));
}

void test_switching_to_controller_drops_a_fleet_scene_pending() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n + 30, 16, makeSettings(1)));
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  requestContent(2);
  TEST_ASSERT_EQUAL_UINT32(n + SCENE_LEAD_FRAMES, readPendingScene().start);
}

void test_a_controllers_channel_switch_keeps_its_scenes_and_sends_them() {
  moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  fake.animationEndpoint.sent.clear();
  rvl::setChannel(1);
  TEST_ASSERT_EQUAL(1, readId(readPendingScene()));
  TEST_ASSERT_EQUAL(1, fake.animationEndpoint.sent.size());
}

void test_in_receiver_mode_the_conveniences_schedule_nothing() {
  uint32_t n = moveToFreshFrame();
  requestContent(1);
  rvl::setOff();
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  TEST_ASSERT_EQUAL(0, updates);
  TEST_ASSERT_EQUAL(0, fake.animationEndpoint.sent.size());
  // Nothing was held either
  runThrough(n + 40);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  TEST_ASSERT_EQUAL(-1, readId(rvl::getCurrentScene()));
}

// The switch to Receiver resets every slot to the boot scene, which is this
// node's own and must not go out when it becomes a controller again
void test_a_controller_toggled_to_receiver_and_back_sends_only_the_preset() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  runThrough(n + 30);
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  fake.animationEndpoint.sent.clear();
  // What the mode control does next
  requestContent(2);
  TEST_ASSERT_EQUAL(1, fake.animationEndpoint.sent.size());
  TEST_ASSERT_EQUAL(PACKET_TYPE_PARAMETRIC_ANIMATION,
      fake.animationEndpoint.sent[0].bytes[6]);
}

void test_a_step_reschedules_the_held_request_over_pending() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  requestContent(2);
  rvl::adjustAnimationClock(3 * FRAME_PERIOD);
  TEST_ASSERT_EQUAL_UINT32(n + 3, rvl::getAnimationFrame());
  RVLScene p = readPendingScene();
  TEST_ASSERT_EQUAL(2, readId(p));
  TEST_ASSERT_EQUAL_UINT32(n + 3 + SCENE_LEAD_FRAMES, p.start);
  TEST_ASSERT_EQUAL(DEFAULT_FADE_FRAMES, p.fade);
  TEST_ASSERT_EQUAL(2, updates);
}

void test_a_step_reschedules_pending_with_its_own_fade() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::scheduleScene(makeScene(n + 30, 40, makeSettings(1)));
  rvl::adjustAnimationClock(2 * FRAME_PERIOD);
  RVLScene p = readPendingScene();
  TEST_ASSERT_EQUAL(1, readId(p));
  TEST_ASSERT_EQUAL_UINT32(n + 2 + SCENE_LEAD_FRAMES, p.start);
  TEST_ASSERT_EQUAL(40, p.fade);
}

// Identical content, so it activates with nothing to dissolve
void test_a_step_reschedules_current_with_its_own_fade() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::scheduleScene(makeScene(n, 24, makeSettings(1)));
  runThrough(n + 30);
  rvl::adjustAnimationClock(-2 * FRAME_PERIOD);
  RVLScene p = readPendingScene();
  TEST_ASSERT_EQUAL(1, readId(p));
  TEST_ASSERT_EQUAL_UINT32(n + 28 + SCENE_LEAD_FRAMES, p.start);
  TEST_ASSERT_EQUAL(24, p.fade);
  runThrough(n + 28 + SCENE_LEAD_FRAMES);
  TEST_ASSERT_EQUAL_UINT32(
      n + 28 + SCENE_LEAD_FRAMES, rvl::getCurrentScene().start);
  TEST_ASSERT_FALSE(rvl::getRenderPlan().fading);
}

// The one scene with no fade is the boot scene, and the sender relies on that
// to keep it off the wire, so a copy of it made by the hook gets the floor
void test_a_step_on_the_boot_scene_schedules_it_with_the_floor() {
  moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::adjustAnimationClock(FRAME_PERIOD);
  RVLScene pending = readPendingScene();
  TEST_ASSERT_EQUAL(-1, readId(pending));
  TEST_ASSERT_EQUAL(MIN_FADE_FRAMES, pending.fade);
}

// A controller's own fleet would otherwise hold its re-scheduled scene under
// the overlap rule
void test_a_step_during_a_fade_reschedules_at_the_fades_end() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::scheduleScene(makeScene(n, 32, makeSettings(1)));
  loopAt(n + 2);
  rvl::adjustAnimationClock(3 * FRAME_PERIOD);
  TEST_ASSERT_EQUAL_UINT32(n + 35, readPendingScene().start);
}

void test_a_receivers_step_drops_its_pending_scene() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n + 30, 16, makeSettings(1)));
  rvl::adjustAnimationClock(-5 * FRAME_PERIOD);
  TEST_ASSERT_FALSE(rvl::getPendingScene().has_value());
  TEST_ASSERT_EQUAL(1, updates);
}

void test_a_fade_keeps_its_amount_across_a_step_either_way() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 64, makeSettings(1)));
  loopAt(n + 10);
  uint8_t amount = rvl::getRenderPlan().amount;
  rvl::adjustAnimationClock(20 * FRAME_PERIOD);
  TEST_ASSERT_EQUAL(amount, rvl::getRenderPlan().amount);
  rvl::adjustAnimationClock(-7 * FRAME_PERIOD);
  auto plan = rvl::getRenderPlan();
  TEST_ASSERT_TRUE(plan.fading);
  TEST_ASSERT_EQUAL(amount, plan.amount);
}

// This node converging on the fleet: its bookkeeping is already in the fleet's
// frames, so it stays, and a boundary crossing just moves to the next frame
void test_a_correction_under_a_frame_changes_nothing() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 64, makeSettings(1)));
  rvl::scheduleScene(makeScene(n + 100, 16, makeSettings(2)));
  loopAt(n + 10, 20);
  uint32_t before = updates;
  rvl::adjustAnimationClock(FRAME_PERIOD - 1);
  auto plan = rvl::getRenderPlan();
  TEST_ASSERT_EQUAL_UINT32(n + 11, plan.frame);
  TEST_ASSERT_EQUAL(11 * 255 / 64, plan.amount);
  rvl::adjustAnimationClock(-(FRAME_PERIOD - 1));
  TEST_ASSERT_EQUAL(10 * 255 / 64, rvl::getRenderPlan().amount);
  TEST_ASSERT_EQUAL(2, readId(readPendingScene()));
  TEST_ASSERT_EQUAL(before, updates);
}

// Scenes every `spacing` frames with `fade`-frame fades, each delivered three
// frames after its start, as a DTIM wait would. Returns each scene's id, the
// frame it activated in and the frame its fade ended
struct Activation {
  int id;
  uint32_t frame;
  uint32_t fadeEnd;
};

std::vector<Activation> runOverdrive(
    uint32_t n, uint32_t spacing, uint8_t fade, uint32_t count) {
  std::vector<Activation> activations;
  int showing = readId(rvl::getCurrentScene());
  for (uint32_t f = n; f < n + spacing * count + 2 * UINT8_MAX; f++) {
    setFrame(f);
    if (f >= n + 3 && (f - n - 3) % spacing == 0) {
      uint32_t k = (f - n - 3) / spacing;
      if (k < count) {
        rvl::scheduleScene(
            makeScene(n + k * spacing, fade, makeSettings(k + 1)));
      }
    }
    rvl::loop();
    if (readId(rvl::getCurrentScene()) != showing) {
      showing = readId(rvl::getCurrentScene());
      // Moves only the clock; the next iteration sets it back
      uint32_t end = f;
      while (readPlanAt(end).fading) {
        end++;
      }
      activations.push_back({showing, f, end});
    }
  }
  return activations;
}

// Scene 0 on arrival, then every third, each 16 frames after its start, which
// is where the one before it ends: fades of 48 frames and a constant lag of
// 16. The last scene sent catches up when the stream stops
void test_an_overdriving_sender_shows_every_third_scene_on_time() {
  uint32_t n = moveToFreshFrame();
  auto activations = runOverdrive(n, 16, 64, 20);
  TEST_ASSERT_EQUAL(8, activations.size());
  TEST_ASSERT_EQUAL(1, activations[0].id);
  TEST_ASSERT_EQUAL_UINT32(n + 3, activations[0].frame);
  for (uint32_t i = 1; i < 7; i++) {
    uint32_t k = 3 * i;
    TEST_ASSERT_EQUAL(k + 1, activations[i].id);
    TEST_ASSERT_EQUAL_UINT32(n + 16 * k + 16, activations[i].frame);
  }
  TEST_ASSERT_EQUAL(20, activations[7].id);
  TEST_ASSERT_EQUAL_UINT32(n + 16 * 18 + 64, activations[7].frame);
  // Every fade ends where its scene intended, start plus fade
  for (auto& activation : activations) {
    TEST_ASSERT_EQUAL_UINT32(
        n + 16 * (activation.id - 1) + 64, activation.fadeEnd);
  }
}

void test_a_sender_at_the_fade_rate_is_never_held() {
  uint32_t n = moveToFreshFrame();
  auto activations = runOverdrive(n, 16, 16, 20);
  TEST_ASSERT_EQUAL(20, activations.size());
  for (uint32_t k = 0; k < 20; k++) {
    TEST_ASSERT_EQUAL(k + 1, activations[k].id);
    TEST_ASSERT_EQUAL_UINT32(n + 16 * k + 3, activations[k].frame);
    // No remainder to shorten, so each runs the floor and ends that late
    TEST_ASSERT_EQUAL_UINT32(
        activations[k].frame + MIN_FADE_FRAMES, activations[k].fadeEnd);
  }
}

void test_late_activations_are_reported_once_a_minute() {
  uint32_t n = moveToFreshFrame();
  loopAt(n + 3);
  rvl::scheduleScene(makeScene(n, 16, makeSettings(1)));
  // Later than a whole fade is a catch-up, not a missed lead
  setFrame(n + 1000);
  rvl::scheduleScene(makeScene(n + 500, 16, makeSettings(2)));
  hopTo(n + 1000 + 60000 / FRAME_PERIOD + 1);
  TEST_ASSERT_TRUE(fake.logged(
      "Late scene activations in the last minute: 1, by up to 3 frames"));
  fake.output.clear();
  hopTo(rvl::getAnimationFrame() + 60000 / FRAME_PERIOD + 1);
  TEST_ASSERT_FALSE(fake.logged("Late scene activations"));
}

void test_a_controller_rekeys_a_scene_older_than_a_day() {
  uint32_t n = moveToFreshFrame();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(1);
  runThrough(n + 30);
  RVLScene before = rvl::getCurrentScene();
  hopTo(before.start + SCENE_MAX_AGE_FRAMES + 1);
  TEST_ASSERT_EQUAL(2, updates);
  uint32_t f = rvl::getAnimationFrame();
  RVLScene p = readPendingScene();
  TEST_ASSERT_EQUAL_UINT32(f + SCENE_LEAD_FRAMES, p.start);
  TEST_ASSERT_EQUAL(before.fade, p.fade);
  TEST_ASSERT_TRUE(p.content == before.content);
  runThrough(f + SCENE_LEAD_FRAMES);
  TEST_ASSERT_EQUAL_UINT32(f + SCENE_LEAD_FRAMES, rvl::getCurrentScene().start);
  TEST_ASSERT_FALSE(rvl::getRenderPlan().fading);
}

void test_a_receiver_takes_a_rekeyed_scene_on_time_and_unchanged() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 16, makeSettings(1)));
  runThrough(n + 30);
  rvl::scheduleScene(makeScene(n + 40, 16, makeSettings(1)));
  runThrough(n + 40);
  TEST_ASSERT_EQUAL_UINT32(n + 40, rvl::getCurrentScene().start);
  TEST_ASSERT_FALSE(rvl::getRenderPlan().fading);
  hopTo(rvl::getAnimationFrame() + 60000 / FRAME_PERIOD + 1);
  TEST_ASSERT_FALSE(fake.logged("Late scene activations"));
}

// Its sender re-keys daily, so a scene is never more than a day old
void test_a_receiver_joining_a_long_running_scene_fades_in() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(
      makeScene(n - SCENE_MAX_AGE_FRAMES - 63, 16, makeSettings(1)));
  auto plan = rvl::getRenderPlan();
  TEST_ASSERT_EQUAL(1, readId(plan.current));
  TEST_ASSERT_EQUAL(-1, readId(plan.previous));
  TEST_ASSERT_TRUE(plan.fading);
}

void test_scenes_and_fades_cross_the_frame_wrap() {
  hopTo(LAST_FRAME - 10);
  rvl::scheduleScene(makeScene(LAST_FRAME - 10, 32, makeSettings(1)));
  // Masked, as a sender that wraps its frame numbers would send it
  rvl::scheduleScene(
      makeScene((LAST_FRAME + 30) & LAST_FRAME, 16, makeSettings(2)));
  uint8_t amount = 0;
  for (uint32_t k = 0; k < 32; k++) {
    auto plan = readPlanAt(LAST_FRAME - 10 + k);
    TEST_ASSERT_TRUE(plan.fading);
    TEST_ASSERT_GREATER_OR_EQUAL(amount, plan.amount);
    amount = plan.amount;
  }
  TEST_ASSERT_FALSE(readPlanAt(LAST_FRAME + 22).fading);
  runThrough(LAST_FRAME + 29);
  TEST_ASSERT_EQUAL(1, readId(rvl::getCurrentScene()));
  loopAt(LAST_FRAME + 30);
  TEST_ASSERT_EQUAL(2, readId(rvl::getCurrentScene()));
}

void test_a_step_across_the_frame_wrap_keeps_the_fade() {
  hopTo(LAST_FRAME - 5);
  rvl::scheduleScene(makeScene(LAST_FRAME - 5, 64, makeSettings(1)));
  loopAt(LAST_FRAME - 2);
  uint8_t amount = rvl::getRenderPlan().amount;
  rvl::adjustAnimationClock(10 * FRAME_PERIOD);
  auto plan = rvl::getRenderPlan();
  TEST_ASSERT_EQUAL_UINT32(7, plan.frame);
  TEST_ASSERT_TRUE(plan.fading);
  TEST_ASSERT_EQUAL(amount, plan.amount);
}

// Time alone can't say a fade is over: 2^26 frames after it ended a one-sided
// comparison reads its end as the future, and a full cycle later its window
// comes around again
void test_a_long_idle_reports_no_fade_and_leaves_the_gate_open() {
  uint32_t n = moveToFreshFrame();
  rvl::scheduleScene(makeScene(n, 16, makeSettings(1)));
  runThrough(n + 16);
  hopTo(n + 16 + (1u << 26) + 1);
  TEST_ASSERT_FALSE(rvl::getRenderPlan().fading);
  hopTo(n + 16 + LAST_FRAME);
  TEST_ASSERT_FALSE(rvl::getRenderPlan().fading);
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  requestContent(2);
  TEST_ASSERT_EQUAL(2, readId(readPendingScene()));
}

int main() {
  rvl::init(&fake);
  rvl::setLinkUpState(true);
  rvl::setDeviceId(LOCAL_ID);
  // The sender is gated on the first sync, and nothing here tests an unsynced
  // clock
  rvl::NetworkState::refreshLocalClockSynchronization();
  rvl::on(EVENT_ANIMATION_UPDATED, countUpdate);
  UNITY_BEGIN();
  RUN_TEST(test_a_future_scene_pends_and_activates_at_its_start);
  RUN_TEST(test_the_amount_runs_over_the_fade_by_frame);
  RUN_TEST(test_a_late_board_fades_over_the_remainder_and_ends_with_the_fleet);
  RUN_TEST(test_a_board_later_than_the_remainder_allows_fades_over_the_floor);
  RUN_TEST(test_a_scene_nearly_half_a_cycle_late_fades_over_the_floor);
  RUN_TEST(test_a_scene_the_node_holds_is_a_resend);
  RUN_TEST(test_a_same_start_with_other_content_is_a_new_scene);
  RUN_TEST(
      test_a_due_scene_displaces_any_pending_and_a_future_one_only_a_future_one);
  RUN_TEST(test_a_channel_toggle_accepts_a_resend_of_the_scene_it_left);
  RUN_TEST(test_the_senders_copy_of_the_scene_just_left_changes_nothing);
  RUN_TEST(test_a_due_scene_waits_out_a_running_fade);
  RUN_TEST(test_a_due_scene_arriving_as_a_fade_ends_supersedes_a_waiting_one);
  RUN_TEST(test_a_scene_starting_inside_a_fade_fades_over_what_remains);
  RUN_TEST(test_a_senders_current_and_pending_in_one_drain_both_land);
  RUN_TEST(test_mid_fade_the_due_scene_of_a_pair_outranks_the_future_one);
  RUN_TEST(test_the_next_scene_arriving_before_the_current_one_still_pends);
  RUN_TEST(test_a_scene_with_the_current_content_has_no_fade);
  RUN_TEST(test_off_fades_in_both_directions);
  RUN_TEST(test_no_fade_is_reported_at_boot);
  RUN_TEST(test_the_update_event_fires_on_acceptance_not_activation);
  RUN_TEST(test_a_controllers_change_starts_a_lead_from_now);
  RUN_TEST(test_changes_during_the_lead_and_fade_are_held_and_the_last_wins);
  RUN_TEST(test_the_gate_opens_in_a_late_iteration_too);
  RUN_TEST(test_off_on_an_off_strip_and_a_dial_turned_back_do_nothing);
  RUN_TEST(test_a_request_back_to_the_pending_scene_clears_the_held_one);
  RUN_TEST(test_a_request_back_to_the_fading_scene_clears_the_held_one);
  RUN_TEST(test_repeating_the_held_request_holds_it);
  RUN_TEST(test_a_receivers_channel_switch_drops_its_pending_scene);
  RUN_TEST(test_switching_to_receiver_returns_to_the_boot_scene);
  RUN_TEST(test_switching_to_controller_drops_a_fleet_scene_pending);
  RUN_TEST(test_a_controllers_channel_switch_keeps_its_scenes_and_sends_them);
  RUN_TEST(test_in_receiver_mode_the_conveniences_schedule_nothing);
  RUN_TEST(
      test_a_controller_toggled_to_receiver_and_back_sends_only_the_preset);
  RUN_TEST(test_a_step_reschedules_the_held_request_over_pending);
  RUN_TEST(test_a_step_reschedules_pending_with_its_own_fade);
  RUN_TEST(test_a_step_reschedules_current_with_its_own_fade);
  RUN_TEST(test_a_step_on_the_boot_scene_schedules_it_with_the_floor);
  RUN_TEST(test_a_step_during_a_fade_reschedules_at_the_fades_end);
  RUN_TEST(test_a_receivers_step_drops_its_pending_scene);
  RUN_TEST(test_a_fade_keeps_its_amount_across_a_step_either_way);
  RUN_TEST(test_a_correction_under_a_frame_changes_nothing);
  RUN_TEST(test_an_overdriving_sender_shows_every_third_scene_on_time);
  RUN_TEST(test_a_sender_at_the_fade_rate_is_never_held);
  RUN_TEST(test_late_activations_are_reported_once_a_minute);
  RUN_TEST(test_a_controller_rekeys_a_scene_older_than_a_day);
  RUN_TEST(test_a_receiver_takes_a_rekeyed_scene_on_time_and_unchanged);
  RUN_TEST(test_a_receiver_joining_a_long_running_scene_fades_in);
  // These move the clock a long way, so they run last
  RUN_TEST(test_scenes_and_fades_cross_the_frame_wrap);
  RUN_TEST(test_a_step_across_the_frame_wrap_keeps_the_fade);
  RUN_TEST(test_a_long_idle_reports_no_fade_and_leaves_the_gate_open);
  return UNITY_END();
}
