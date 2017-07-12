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
#include "value.h"

#define CONTENT_PADDING 3

namespace BrightnessControl {

  void drawVerticalBar(SSD1306Brzo display, int x, int y, int w, int h, double percent) {
    const int fillHeight = (h - 2) * percent;
    const int emptyHeight = h - 2 - fillHeight;

    // Draw the border. drawRect w is x + w, but fillRect is x + w - 1, so normalize here
    display.drawRect(x, y, w - 1, h);

    // Draw the filled section
    if (fillHeight != 0) {
      display.fillRect(
        x + 1,
        y + 1 + emptyHeight,
        w - 3,
        fillHeight);
    }
  }

  void drawBrightnessIcon(SSD1306Brzo display, int x, int y, int size) {
    int midX = x + size / 2;
    int midY = y + size / 2;
    int indent = size / 8;
    int radius = size / 3;

    display.drawLine(midX, y, midX, y + size - 1);
    display.drawLine(x, midY, x + size - 1, midY);
    display.drawLine(x + indent, y + indent, x + size - 1 - indent, y + size - 1 - indent);
    display.drawLine(x + indent, y + size - 1 - indent, x + size - 1 - indent, y + 1);
    display.fillCircle(midX + 1, midY, radius);
  }

  void render(SSD1306Brzo display, byte x, byte y, byte width, byte height, bool isSelected, byte brightness) {

    if (isSelected) {
      display.drawRect(x, y, width, height);
    }

    drawBrightnessIcon(display, x + 2, height - width, width - 4);
    drawVerticalBar(display, x + 2, y + 2, width - 4, height - width - 5, (double)brightness / 255);
  }

}
