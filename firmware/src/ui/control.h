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

#ifndef UI_CONTROL_H_
#define UI_CONTROL_H_

#include <Arduino.h>
#include <vector>

namespace Control {

enum class ControlType {
  Range,
  List
};

class Control {
 public:
  const char* label;
  ControlType type;
  void (*updateValue)(uint8_t newValue);
  virtual void increaseValue() {}
  virtual void decreaseValue() {}
};

class ListControl : public Control {
 public:
  std::vector<const char*> values;
  uint8_t selectedValueIndex = 0;

  ListControl(
    const char* listLabel,
    std::vector<const char*> listValues,
    uint8_t defaultValueIndex,
    void (*updateValueCallback)(uint8_t newValue)
  ) {
    this->type = ControlType::List;
    this->label = listLabel;
    this->values = listValues;
    this->updateValue = updateValueCallback;
    this->selectedValueIndex = defaultValueIndex;
  }

  void increaseValue() {
    if (this->selectedValueIndex == this->values.size() - 1) {
      this->selectedValueIndex = 0;
    } else {
      this->selectedValueIndex++;
    }
    this->updateValue(this->selectedValueIndex);
  }

  void decreaseValue() {
    if (this->selectedValueIndex == 0) {
      this->selectedValueIndex = this->values.size() - 1;
    } else {
      this->selectedValueIndex--;
    }
    this->updateValue(this->selectedValueIndex);
  }
};

class RangeControl : public Control {
 public:
  uint8_t value;
  uint8_t min;
  uint8_t max;
  uint8_t (*getValue)();

  RangeControl(
    const char* rangeLabel,
    uint8_t rangeMin,
    uint8_t rangeMax,
    uint8_t defaultValue,
    void (*updateValueCallback)(uint8_t newValue),
    uint8_t (*getValueMethod)()
  ) {
    this->type = ControlType::Range;
    this->label = rangeLabel;
    this->updateValue = updateValueCallback;
    this->min = rangeMin;
    this->max = rangeMax;
    this->value = defaultValue;
    this->getValue = getValueMethod;
  }

  void increaseValue() {
    if (this->value < this->max) {
      this->value++;
      this->updateValue(this->value);
    }
  }

  void decreaseValue() {
    if (this->value > this->min) {
      this->value--;
      this->updateValue(this->value);
    }
  }
};

}  // namespace Control

#endif  // UI_CONTROL_H_
