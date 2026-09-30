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
#include <stdio.h>
#include <unity.h>

// Frame numbers run 0..FRAME_COUNT - 1, and subtractFrames's range is half of
// that either side of zero
const uint32_t FRAME_COUNT = 1u << 27;
const int32_t HALF = 1 << 26;

void setAnimationClock(uint32_t time) {
  rvl::adjustAnimationClock(
      static_cast<int32_t>(time - rvl::getAnimationClock()));
}

void setUp() {}

void tearDown() {}

void test_a_frame_is_the_animation_clock_over_the_frame_period() {
  setAnimationClock(0);
  TEST_ASSERT_EQUAL_UINT32(0, rvl::getAnimationFrame());
  setAnimationClock(FRAME_PERIOD - 1);
  TEST_ASSERT_EQUAL_UINT32(0, rvl::getAnimationFrame());
  setAnimationClock(FRAME_PERIOD);
  TEST_ASSERT_EQUAL_UINT32(1, rvl::getAnimationFrame());
}

void test_frame_numbers_wrap_with_the_animation_clock() {
  setAnimationClock(UINT32_MAX);
  uint32_t last = rvl::getAnimationFrame();
  TEST_ASSERT_EQUAL_UINT32(FRAME_COUNT - 1, last);
  fake.clock++;
  TEST_ASSERT_EQUAL_UINT32(0, rvl::getAnimationFrame());
  TEST_ASSERT_EQUAL_INT32(
      1, rvl::subtractFrames(rvl::getAnimationFrame(), last));
}

void test_frame_differences_are_signed() {
  TEST_ASSERT_EQUAL_INT32(0, rvl::subtractFrames(7, 7));
  TEST_ASSERT_EQUAL_INT32(2, rvl::subtractFrames(7, 5));
  TEST_ASSERT_EQUAL_INT32(-2, rvl::subtractFrames(5, 7));
}

// The clock's idiom, static_cast<int32_t>(a - b), reads the first of these as
// FRAME_COUNT - 1 frames in the past
void test_a_frame_past_the_wrap_is_one_ahead() {
  TEST_ASSERT_EQUAL_INT32(1, rvl::subtractFrames(0, FRAME_COUNT - 1));
  TEST_ASSERT_EQUAL_INT32(-1, rvl::subtractFrames(FRAME_COUNT - 1, 0));
}

void test_the_signed_range_is_half_the_cycle() {
  TEST_ASSERT_EQUAL_INT32(HALF - 1, rvl::subtractFrames(HALF - 1, 0));
  TEST_ASSERT_EQUAL_INT32(-HALF, rvl::subtractFrames(HALF, 0));
  TEST_ASSERT_EQUAL_INT32(-HALF, rvl::subtractFrames(0, HALF));
  TEST_ASSERT_EQUAL_INT32(HALF - 1, rvl::subtractFrames(0, HALF + 1));
  TEST_ASSERT_EQUAL_INT32(-(HALF - 1), rvl::subtractFrames(0, HALF - 1));
}

void test_differences_hold_from_any_frame() {
  const uint32_t bases[] = {0, 1, HALF - 1, HALF, FRAME_COUNT - 1};
  const int32_t deltas[] = {-HALF, -HALF + 1, -1, 0, 1, HALF - 2, HALF - 1};
  char message[48];
  for (uint32_t base : bases) {
    for (int32_t delta : deltas) {
      uint32_t frame = (base + delta) % FRAME_COUNT;
      snprintf(message, sizeof(message), "base %u, delta %d", base, delta);
      TEST_ASSERT_EQUAL_INT32_MESSAGE(
          delta, rvl::subtractFrames(frame, base), message);
    }
  }
}

// A sum like start + fade can pass FRAME_COUNT without being masked, and still
// compares correctly with a frame that has wrapped
void test_an_unmasked_sum_compares_with_a_wrapped_frame() {
  uint32_t end = (FRAME_COUNT - 10) + 16;
  TEST_ASSERT_EQUAL_INT32(3, rvl::subtractFrames(end, 3));
  TEST_ASSERT_EQUAL_INT32(-3, rvl::subtractFrames(3, end));
}

int main() {
  rvl::init(&fake);
  UNITY_BEGIN();
  RUN_TEST(test_a_frame_is_the_animation_clock_over_the_frame_period);
  RUN_TEST(test_frame_numbers_wrap_with_the_animation_clock);
  RUN_TEST(test_frame_differences_are_signed);
  RUN_TEST(test_a_frame_past_the_wrap_is_one_ahead);
  RUN_TEST(test_the_signed_range_is_half_the_cycle);
  RUN_TEST(test_differences_hold_from_any_frame);
  RUN_TEST(test_an_unmasked_sum_compares_with_a_wrapped_frame);
  return UNITY_END();
}
