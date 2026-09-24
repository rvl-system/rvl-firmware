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

#ifndef TIMING_STATS_H_
#define TIMING_STATS_H_

#include <rvl.hpp>
#include <stdint.h>

#define NUM_LOOP_SAMPLES 60

struct TimingStats {
  uint32_t samples[NUM_LOOP_SAMPLES];
  uint8_t count = 0;

  void record(uint32_t elapsed) {
    if (count < NUM_LOOP_SAMPLES) {
      samples[count++] = elapsed;
    }
  }

  void log(const char* label) {
    if (count < NUM_LOOP_SAMPLES) {
      return;
    }
    count = 0;
    uint32_t sum = 0;
    uint32_t min = UINT32_MAX;
    uint32_t max = 0;
    for (uint8_t i = 0; i < NUM_LOOP_SAMPLES; i++) {
      sum += samples[i];
      if (samples[i] < min) {
        min = samples[i];
      }
      if (samples[i] > max) {
        max = samples[i];
      }
    }
    rvl::debug("%s stats: Avg=%u Min=%u Max=%u", label, sum / NUM_LOOP_SAMPLES,
        min, max);
  }
};

#endif // TIMING_STATS_H_
