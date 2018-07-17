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
#include "./ui/presets/rainbow.h"
#include "./codes.h"
#include "./ui/ui_state.h"
#include "./state.h"

namespace Rainbow {

void calculateWaveParameters() {
  State::WaveSettings newSettings;
  newSettings.waves[0].h.a = 255;
  newSettings.waves[0].h.w_t = UIState::presetValues[Codes::Preset::Rainbow][Codes::RainbowPresetValues::Rate];
  newSettings.waves[0].h.w_x = 2;
  newSettings.waves[0].s.b = 255;
  newSettings.waves[0].v.b = 255;
  newSettings.waves[0].a.b = 255;
  State::setWaveParameters(&newSettings);
}

}  // namespace Rainbow