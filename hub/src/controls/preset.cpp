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

#include <Arduino.h>
#include "./controls/preset.h"
#include "./controls/value.h"

#define CONTENT_PADDING 3
#define LABEL_WIDTH 49

namespace PresetControl {

void render(SSD1306Brzo display, byte x, byte y, byte width, byte height, bool isSelected, const char* preset) {
  if (isSelected) {
    display.drawRect(x, y, width, height);
  }

  display.drawString(x + CONTENT_PADDING, y, "PRESET: ");
  display.drawString(x + CONTENT_PADDING + LABEL_WIDTH, y, preset);
}

}  // namespace PresetControl
