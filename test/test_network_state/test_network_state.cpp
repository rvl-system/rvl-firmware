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
#include <rvl/protocols/network_state.hpp>
#include <unity.h>

#define LOCAL_ID 10
#define CONTROLLER_ID 20
#define OTHER_CONTROLLER_ID 21

namespace NetworkState = rvl::NetworkState;

#define TEST_ASSERT_RENDER_STATE(expected)                                     \
  TEST_ASSERT_EQUAL_INT(static_cast<int>(rvl::RenderState::expected),          \
      static_cast<int>(rvl::getRenderState()))

void loopAt(uint32_t time) {
  fake.clock = time;
  rvl::loop();
}

void forgetController() {
  rvl::setChannel(1);
  rvl::setChannel(0);
}

void becomeCurrent() {
  rvl::setLinkUpState(true);
  rvl::setDeviceId(LOCAL_ID);
  NetworkState::refreshLocalClockSynchronization();
  NetworkState::isControllerNode(CONTROLLER_ID);
  rvl::loop();
}

void setUp() {
  fake.output.clear();
}

void tearDown() {}

// Must run first: nothing has synced or been heard since boot
void test_unknown_at_boot() {
  TEST_ASSERT_RENDER_STATE(Unknown);
  rvl::setLinkUpState(true);
  rvl::setDeviceId(LOCAL_ID);
  rvl::loop();
  TEST_ASSERT_RENDER_STATE(Unknown);
}

// Must run before anything syncs the clock, which nothing unsyncs
void test_a_controller_without_a_clock_is_unknown() {
  NetworkState::isControllerNode(CONTROLLER_ID);
  rvl::loop();
  TEST_ASSERT_RENDER_STATE(Unknown);
}

void test_current_once_connected_synced_and_a_controller_is_heard() {
  becomeCurrent();
  TEST_ASSERT_RENDER_STATE(Current);
  TEST_ASSERT_TRUE(fake.logged("Render state is now current"));
}

void test_stale_once_the_controller_is_quiet_for_10_s() {
  becomeCurrent();
  uint32_t heard = fake.clock;
  loopAt(heard + 9999);
  TEST_ASSERT_RENDER_STATE(Current);
  loopAt(heard + 10000);
  TEST_ASSERT_RENDER_STATE(Stale);
  TEST_ASSERT_TRUE(fake.logged("Render state is now stale"));
}

void test_still_stale_after_the_clock_window_lapses() {
  becomeCurrent();
  loopAt(fake.clock + 60000);
  TEST_ASSERT_FALSE(rvl::getClockSyncedState());
  TEST_ASSERT_RENDER_STATE(Stale);
}

void test_a_receiver_that_loses_its_link_goes_stale() {
  becomeCurrent();
  rvl::setLinkUpState(false);
  rvl::loop();
  TEST_ASSERT_RENDER_STATE(Stale);
}

void test_a_channel_change_is_unknown_until_a_controller_is_heard() {
  becomeCurrent();
  rvl::setChannel(1);
  rvl::loop();
  TEST_ASSERT_RENDER_STATE(Unknown);
  NetworkState::isControllerNode(OTHER_CONTROLLER_ID);
  rvl::loop();
  TEST_ASSERT_RENDER_STATE(Current);
  rvl::setChannel(0);
}

// A board may have heard a controller long before it became a receiver
void test_a_switch_to_receiver_is_unknown_until_a_controller_is_heard() {
  becomeCurrent();
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::loop();
  TEST_ASSERT_RENDER_STATE(Current);
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  rvl::loop();
  TEST_ASSERT_RENDER_STATE(Unknown);
}

void test_a_controller_is_always_current() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  rvl::setLinkUpState(false);
  loopAt(fake.clock + 120000);
  TEST_ASSERT_RENDER_STATE(Current);
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
}

void test_the_first_controller_heard_is_adopted() {
  forgetController();
  TEST_ASSERT_TRUE(NetworkState::isControllerNode(CONTROLLER_ID));
  TEST_ASSERT_FALSE(NetworkState::isControllerNode(OTHER_CONTROLLER_ID));
}

void test_a_controller_that_keeps_sending_keeps_its_place() {
  forgetController();
  uint32_t start = fake.clock;
  NetworkState::isControllerNode(CONTROLLER_ID);
  fake.clock = start + 9000;
  TEST_ASSERT_TRUE(NetworkState::isControllerNode(CONTROLLER_ID));
  fake.clock = start + 15000;
  TEST_ASSERT_FALSE(NetworkState::isControllerNode(OTHER_CONTROLLER_ID));
}

void test_another_controller_takes_over_after_10_s_of_quiet() {
  forgetController();
  uint32_t start = fake.clock;
  NetworkState::isControllerNode(CONTROLLER_ID);
  fake.clock = start + 10001;
  TEST_ASSERT_TRUE(NetworkState::isControllerNode(OTHER_CONTROLLER_ID));
  TEST_ASSERT_FALSE(NetworkState::isControllerNode(CONTROLLER_ID));
}

void test_a_controller_is_active_for_10_s_after_it_was_heard() {
  forgetController();
  TEST_ASSERT_FALSE(NetworkState::isControllerActive());
  uint32_t start = fake.clock;
  NetworkState::isControllerNode(CONTROLLER_ID);
  fake.clock = start + 9999;
  TEST_ASSERT_TRUE(NetworkState::isControllerActive());
  fake.clock = start + 10000;
  TEST_ASSERT_FALSE(NetworkState::isControllerActive());
}

void test_clock_sync_is_active_for_60_s_after_a_correction() {
  uint32_t start = fake.clock;
  NetworkState::refreshLocalClockSynchronization();
  fake.clock = start + 59999;
  TEST_ASSERT_TRUE(NetworkState::isClockSynchronizationActive());
  fake.clock = start + 60000;
  TEST_ASSERT_FALSE(NetworkState::isClockSynchronizationActive());
}

void test_a_controller_never_follows_another() {
  rvl::setDeviceMode(rvl::DeviceMode::Controller);
  TEST_ASSERT_FALSE(NetworkState::isControllerNode(CONTROLLER_ID));
  rvl::setDeviceMode(rvl::DeviceMode::Receiver);
}

void test_both_windows_hold_across_the_clock_wrap() {
  forgetController();
  fake.clock = 0xFFFFF000;
  NetworkState::isControllerNode(CONTROLLER_ID);
  NetworkState::refreshLocalClockSynchronization();
  fake.clock = 0x1000;
  TEST_ASSERT_TRUE(NetworkState::isControllerActive());
  TEST_ASSERT_TRUE(NetworkState::isClockSynchronizationActive());
}

int main() {
  rvl::init(&fake);
  UNITY_BEGIN();
  RUN_TEST(test_unknown_at_boot);
  RUN_TEST(test_a_controller_without_a_clock_is_unknown);
  RUN_TEST(test_current_once_connected_synced_and_a_controller_is_heard);
  RUN_TEST(test_stale_once_the_controller_is_quiet_for_10_s);
  RUN_TEST(test_still_stale_after_the_clock_window_lapses);
  RUN_TEST(test_a_receiver_that_loses_its_link_goes_stale);
  RUN_TEST(test_a_channel_change_is_unknown_until_a_controller_is_heard);
  RUN_TEST(test_a_switch_to_receiver_is_unknown_until_a_controller_is_heard);
  RUN_TEST(test_a_controller_is_always_current);
  RUN_TEST(test_the_first_controller_heard_is_adopted);
  RUN_TEST(test_a_controller_that_keeps_sending_keeps_its_place);
  RUN_TEST(test_another_controller_takes_over_after_10_s_of_quiet);
  RUN_TEST(test_a_controller_is_active_for_10_s_after_it_was_heard);
  RUN_TEST(test_clock_sync_is_active_for_60_s_after_a_correction);
  RUN_TEST(test_a_controller_never_follows_another);
  RUN_TEST(test_both_windows_hold_across_the_clock_wrap);
  return UNITY_END();
}
