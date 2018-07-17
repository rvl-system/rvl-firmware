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

#ifndef UI_CONTROLS_BASE_CONTROLS_H_
#define UI_CONTROLS_BASE_CONTROLS_H_

#include <Arduino.h>
#include <vector>
#include "./ui/controls/control.h"

namespace BaseControls {

extern Control::RangeControl brightnessControl;
// extern Control::ListControl wifiControl;
extern Control::ListControl modeControl;
extern Control::ListControl presetControl;

}  // namespace BaseControls

#endif  // UI_CONTROLS_BASE_CONTROLS_H_
