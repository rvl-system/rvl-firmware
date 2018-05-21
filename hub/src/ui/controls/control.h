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

#ifndef UI_CONTROLS_CONTROL_H_
#define UI_CONTROLS_CONTROL_H_

#include <Arduino.h>
#include <vector>

namespace Control {

enum class ControlType {
  Range,
  List,
  Static
};

// TODO(nebrius): Convert const char* to std::string? Depending on memory size?

class Control {
 protected:
  void (*updateHandler)(uint8);
 public:
  const char* label;
  ControlType type;
  void updateValue(uint8 newValue) {
    (this->updateHandler)(newValue);
  }
};

class ListControl : public Control {
 public:
  ListControl(
    void (*updateHandler)(uint8),
    const char* listLabel,
    std::vector<const char*> listValues,
    uint8 listSelectedValueIndex
  ) {
    this->updateHandler = updateHandler;
    this->type = ControlType::List;
    this->label = listLabel;
    this->values = listValues;
    this->selectedValueIndex = listSelectedValueIndex;
  }
  std::vector<const char*> values;
  uint8 selectedValueIndex;
};

class RangeControl : public Control {
 public:
  RangeControl(void (*updateHandler)(uint8), const char* rangeLabel, uint8 rangeValue) {
    this->updateHandler = updateHandler;
    this->type = ControlType::Range;
    this->label = rangeLabel;
    this->value = rangeValue;
  }
  uint8 value;
};

}  // namespace Control

#endif  // UI_CONTROLS_CONTROL_H_
