/*
Copyright (c) 2017 Bryan Hughes <bryan@nebri.us>

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

#include "./state.hpp"
#include "./codes.hpp"
#include <Arduino.h>

namespace State {

void onWaveSettingsUpdated() {
  rvl::emit(Codes::EventType::AnimationChange);
}

void init() {
  rvl::setPowerState(true);
  rvl::on(EVENT_WAVE_SETTINGS_UPDATED, onWaveSettingsUpdated);
  rvl::info("State initialized");
}

void loop() {
}

} // namespace State
