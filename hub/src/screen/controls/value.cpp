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
#include "./screen/controls/value.h"

#define CONTENT_PADDING 3
#define LABEL_WIDTH 32

namespace ValueControl {

void render(
  SSD1306Brzo display,
  byte x,
  byte y,
  byte width,
  byte height,
  bool isSelected,
  const char* label,
  double value
) {
  if (isSelected) {
    display.drawRect(x, y, width, height);
  }

  display.drawString(x + CONTENT_PADDING, y, label);

  // Calculate the bar parameters
  int barX = x + CONTENT_PADDING + LABEL_WIDTH;
  int barY = y + CONTENT_PADDING;
  int barWidth = width - LABEL_WIDTH - 2 * CONTENT_PADDING;
  int barHeight = height - 2 * CONTENT_PADDING;

  // Draw the border
  display.drawRect(barX, barY, barWidth, barHeight);

  // Draw the filled section
  int fillWidth = (barWidth - 2) * (value);
  if (fillWidth != 0) {
    display.fillRect(
      barX + 1,
      barY + 1,
      fillWidth,
      barHeight - 2);
  }
}

}  // namespace ValueControl
