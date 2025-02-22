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

#ifndef UI_PRESETS_PRESET_CONTROL_SET_H_
#define UI_PRESETS_PRESET_CONTROL_SET_H_

#include "./ui/control.hpp"
#include <Arduino.h>
#include <vector>

class PresetControlSet {
public:
  std::vector<Control::Control*> controls;
  virtual void updateWave() = 0;
};

#endif // UI_PRESETS_PRESET_CONTROL_SET_H_
