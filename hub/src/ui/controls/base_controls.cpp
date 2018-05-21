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
#include "./ui/controls/base_controls.h"

namespace BaseControls {

void updateBrightnessValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::RangeControl brightnessControl(updateBrightnessValue, "BRT", 0);

void updateWifiValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::ListControl wifiControl(updateWifiValue, "WIFI", { "RVL1", "RVL2", "RVL3", "RVL4" }, 0);

void updateModeValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::ListControl modeControl(updateModeValue, "MODE", { "Controller", "Receiver" }, 0);

void updatePresetValue(uint8 newValue) {
  // TODO(nebrius): implement me
}
Control::ListControl presetControl(updatePresetValue, "PRST", { "Rainbow", "Pulse", "Wave" }, 0);

}  // namespace BaseControls
