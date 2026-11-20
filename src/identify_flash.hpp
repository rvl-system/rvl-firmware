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

#ifndef IDENTIFY_FLASH_H_
#define IDENTIFY_FLASH_H_

#include <rvl.hpp>
#include <rvl/config.hpp>
#include <stdint.h>

// One flash, lit for its first half
#define IDENTIFY_FLASH_PERIOD 1000
#define IDENTIFY_FLASHES_PER_ROUND 4

// Dark, then a round's flashes in the order shown: white marks its start. The
// outputs' color tables are indexed by these
enum class IdentifyFlash : uint8_t { Dark, White, Red, Green, Blue };

static_assert(
    IDENTIFY_DURATION % (IDENTIFY_FLASHES_PER_ROUND * IDENTIFY_FLASH_PERIOD) ==
    0);

inline IdentifyFlash getIdentifyFlash() {
  uint32_t elapsed = rvl::getIdentifyElapsedTime();
  // Past the end too: a caller's isIdentifying() read the clock before this did
  if (elapsed >= IDENTIFY_DURATION ||
      elapsed % IDENTIFY_FLASH_PERIOD >= IDENTIFY_FLASH_PERIOD / 2)
  {
    return IdentifyFlash::Dark;
  }
  return static_cast<IdentifyFlash>(
      1 + elapsed / IDENTIFY_FLASH_PERIOD % IDENTIFY_FLASHES_PER_ROUND);
}

#endif // IDENTIFY_FLASH_H_
