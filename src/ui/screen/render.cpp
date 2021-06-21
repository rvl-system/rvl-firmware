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

#ifdef HAS_UI

#include "./ui/screen/render.hpp"
#include "../../config.hpp"
#include "./ui/control.hpp"
#include "./ui/screen/icons.hpp"
#include "SSD1306Wire.h"
#include <Arduino.h>
#include <Wire.h>
#include <rvl-wifi.hpp>

namespace Render {

SSD1306Wire display(LCD_ADDRESS, LCD_SDA, LCD_SCL);

struct WindowState {
  uint8_t windowStart = 0; // from 0 to numEntries * ROW_HEIGHT - WINDOW_HEIGHT
};

WindowState windowStates[2];

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SCROLLBAR_WIDTH 3
#define SCROLLBAR_HEIGHT 24

#define ROW_HEIGHT 32
#define ROW_START 21
#define ROW_BORDER_PADDING 3
#define ROW_END (SCREEN_WIDTH - SCROLLBAR_WIDTH - ROW_BORDER_PADDING - 5)

#define ICON_SIZE 16

void init() {
  display.init();
#ifdef INVERT_DISPLAY
  display.flipScreenVertically();
#endif
  display.clear();
  display.setFont(ArialMT_Plain_10);
}

void renderScrollBar(uint8_t numEntries, uint8_t windowStart) {
  if (numEntries > SCREEN_HEIGHT / ROW_HEIGHT) {
    uint8_t y = (SCREEN_HEIGHT - SCROLLBAR_HEIGHT - 1) * windowStart /
        (numEntries * ROW_HEIGHT - SCREEN_HEIGHT);
    // Can happen when changing the preset and the last items disappear
    if (y > SCREEN_HEIGHT - SCROLLBAR_HEIGHT) {
      y = SCREEN_HEIGHT - SCROLLBAR_HEIGHT;
    }
    display.fillRect(
        SCREEN_WIDTH - SCROLLBAR_WIDTH, y, SCROLLBAR_WIDTH, SCROLLBAR_HEIGHT);
  }
}

void renderSelectedEntryBox(uint8_t rowStart) {
  display.drawRect(ROW_START - ROW_BORDER_PADDING, rowStart,
      ROW_END - ROW_START + ROW_BORDER_PADDING * 2 + 2, ROW_HEIGHT - 1);
}

void renderEntry(Control::Control* entry, uint8_t labelY) {
  uint8_t controlY = labelY + ROW_HEIGHT / 2;
  display.drawString(ROW_START, labelY, entry->label);
  if (entry->type == Control::ControlType::List) {
    auto* listEntry = static_cast<Control::ListControl*>(entry);
    display.drawString(ROW_START, controlY, "<");
    display.drawStringMaxWidth(ROW_START + 8, controlY,
        ROW_END - ROW_START - 10,
        listEntry->values[listEntry->selectedValueIndex]);
    display.drawString(ROW_END - 3, controlY, ">");
  } else if (entry->type == Control::ControlType::Range) {
    auto* rangeEntry = static_cast<Control::RangeControl*>(entry);
    uint8_t rangeValue = rangeEntry->value;
    if (rangeEntry->getValue != NULL) {
      rangeValue = rangeEntry->getValue();
    }
    uint8_t progress = 100 * (rangeValue - rangeEntry->min) /
        (rangeEntry->max - rangeEntry->min);
    display.drawProgressBar(
        ROW_START, controlY + 2, ROW_END - ROW_START + 1, 8, progress);
  } else if (entry->type == Control::ControlType::Label) {
    auto* labelEntry = static_cast<Control::LabelControl*>(entry);
    if (labelEntry->getValue != NULL) {
      char labelBuffer[] = "                  ";
      labelEntry->getValue(labelBuffer);
      display.drawString(ROW_START + 5, controlY, labelBuffer);
    } else {
      display.drawString(ROW_START + 5, controlY, "LABEL ERROR");
    }
  }
}

void renderEntrySet(std::vector<Control::Control*>* entries,
    uint8_t selectedTab, uint8_t selectedEntry) {
  if (selectedEntry * ROW_HEIGHT < windowStates[selectedTab].windowStart) {
    windowStates[selectedTab].windowStart = selectedEntry * ROW_HEIGHT;
  } else if (selectedEntry * ROW_HEIGHT >
      windowStates[selectedTab].windowStart + SCREEN_HEIGHT - ROW_HEIGHT)
  {
    windowStates[selectedTab].windowStart =
        (selectedEntry + 1) * ROW_HEIGHT - SCREEN_HEIGHT;
  }
  for (uint8_t i = 0; i < entries->size(); i++) {
    if (i * ROW_HEIGHT >= windowStates[selectedTab].windowStart &&
        i * ROW_HEIGHT < windowStates[selectedTab].windowStart + SCREEN_HEIGHT)
    {
      renderEntry((*entries)[i],
          i * ROW_HEIGHT - windowStates[selectedTab].windowStart);
    }
  }
  renderSelectedEntryBox(
      selectedEntry * ROW_HEIGHT - windowStates[selectedTab].windowStart);
  renderScrollBar(entries->size(), windowStates[selectedTab].windowStart);
}

void renderIcon(Icons::StatusIcon* icon, uint8_t row) {
  for (uint8_t x = 0; x < ICON_SIZE; x++) {
    for (uint8_t y = 0; y < ICON_SIZE; y++) {
      if (icon->data[y][x] == 1) {
        display.setPixel(x, y + row * (ICON_SIZE + 4));
      }
    }
  }
}

void renderIconSet(std::list<Icons::StatusIcon*>* icons) {
  uint8_t i = 0;
  for (auto& icon : *icons) {
    renderIcon(icon, i);
    i++;
  }
}

void render(std::vector<Control::Control*>* entries, uint8_t selectedTab,
    uint8_t selectedEntry, std::list<Icons::StatusIcon*>* icons) {
  display.clear();
  display.setColor(BLACK);
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  display.setColor(WHITE);

  renderIconSet(icons);
  renderEntrySet(entries, selectedTab, selectedEntry);

  display.display();
}

} // namespace Render

#endif // HAS_UI
