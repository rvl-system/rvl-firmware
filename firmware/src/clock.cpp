/*
Copyright (c) 2018 Bryan Hughes <bryan@nebri.us>

This file is part of Raver Lights.

Raver Lights is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Raver Lights is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Raver Lights.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include <RTClib.h>
#include "./clock.h"
#include "./state.h"

namespace Clock {

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void init() {
  if (!rtc.begin()) {
    State::getLogger()->debug("Could not find RTC module, clock information will not be available");
  }
  State::getLogger()->info("Clock initialized");
}

void loop() {
  DateTime now = rtc.now();
  uint8_t hour = now.hour();
  uint8_t minute = now.minute();
  if (State::getHour() != hour || State::getMinute() != minute) {
    State::getLogger()->debug("Setting time to %d:%d", hour, minute);
    State::setTime(hour, minute);
  }
}

}  // namespace Clock
