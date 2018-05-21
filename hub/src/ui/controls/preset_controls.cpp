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
#include <vector>
#include "./ui/controls/control.h"
#include "./ui/controls/preset_controls.h"

namespace PresetControls {

// Rainbow preset entries

void updateRainbowRateValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl rainbowRateControl(updateRainbowRateValue, "RATE", 0);

// Pulse preset entries

void updatePulseRateValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl pulseRateControl(updatePulseRateValue, "RATE", 0);

void updatePulseHueValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl pulseHueControl(updatePulseHueValue, "HUE", 0);

void updatePulseSaturationValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl pulseSaturationControl(updatePulseSaturationValue, "STR", 0);

// Wave preset entries

void updateWaveRateValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl waveRateControl(updateWaveRateValue, "RATE", 0);

void updateWaveForegroundHueValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl waveForegroundHueControl(updateWaveForegroundHueValue, "HUEF", 0);

void updateWaveForegroundSaturationValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl waveForegroundSaturationControl(updateWaveForegroundSaturationValue, "STRF", 0);

void updateWaveBackgroundHueValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl waveBackgroundHueControl(updateWaveBackgroundHueValue, "HUEB", 0);

void updateWaveBackgroundSaturationValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl waveBackgroundSaturationControl(updateWaveBackgroundSaturationValue, "STRB", 0);

}  // namespace PresetControls
