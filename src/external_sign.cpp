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

#include <SparkFun_Qwiic_Relay.h>
#include "./external_sign.h"
#include "./state.h"

namespace ExternalSign {

#define RELAY_ADDR 0x18  // Alternate address 0x19

Qwiic_Relay relay(RELAY_ADDR);

void init() {
  if (!relay.begin()) {
    State::getLogger()->debug("Could not connect to relay.");
  }
  State::getLogger()->info("External sign initialized");
}

void loop() {
  if (State::getTimerState()) {
    relay.turnRelayOn();
  } else {
    relay.turnRelayOff();
  }
}

}  // namespace ExternalSign

