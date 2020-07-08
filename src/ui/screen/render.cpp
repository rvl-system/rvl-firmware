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

#include <rvl-wifi.h>
#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "./ui/screen/render.h"
#include "./ui/screen/icons.h"
#include "./ui/control.h"
#include "../../config.h"

namespace Render {

SSD1306Wire display(LCD_ADDRESS, LCD_SDA, LCD_SCL);

struct WindowState {
  uint8_t previousSelectedEntry = 0;  // from 0 to numEntries
  uint8_t entryWindowStart = 0;   // from 0 to numEntries
  uint8_t selectedEntryRow = 0;  // from 0 to 3
};

WindowState windowStates[2];

void init() {
  display.init();
#ifdef INVERT_DISPLAY
  display.flipScreenVertically();
#endif
  display.clear();
  display.setFont(ArialMT_Plain_10);
}

void renderScrollBar(uint8_t numEntries, uint8_t windowStart) {
  if (numEntries > 4) {
    uint8_t scrollBarHeight = 24;
    uint8_t y = (64 - scrollBarHeight) * windowStart / (numEntries - 4);
    if (y > 64 - scrollBarHeight) {  // Can happen when changing the preset and the last items disappear
      y = 64 - scrollBarHeight;
    }
    display.fillRect(125, y, 3, 24);
  }
}

void renderSelectedEntryBox(uint8_t row) {
  display.drawRect(18, row * 16, 105, 15);
}

void renderEntry(Control::Control* entry, uint8_t row) {
  uint8_t textY = row * 16 + 1;
  if (entry->type == Control::ControlType::List) {
    display.drawString(21, textY, entry->label);
    auto listEntry = static_cast<Control::ListControl*>(entry);
    display.drawString(52, textY, "<");
    display.drawStringMaxWidth(60, textY, 56, listEntry->values[listEntry->selectedValueIndex]);
    display.drawString(115, textY, ">");
  } else if (entry->type == Control::ControlType::Range) {
    display.drawString(21, textY, entry->label);
    auto rangeEntry = static_cast<Control::RangeControl*>(entry);
    uint8_t rangeValue = rangeEntry->value;
    if (rangeEntry->getValue != NULL) {
      rangeValue = rangeEntry->getValue();
    }
    uint8_t progress = 100 * (rangeValue - rangeEntry->min) / (rangeEntry->max - rangeEntry->min);
    display.drawProgressBar(52, row * 16 + 3, 67, 8, progress);
  } else if (entry->type == Control::ControlType::Label) {
    auto labelEntry = static_cast<Control::LabelControl*>(entry);
    if (labelEntry->getValue != NULL) {
      char labelBuffer[] = "                  ";
      labelEntry->getValue(labelBuffer);
      display.drawString(21, textY, labelBuffer);
    } else {
      display.drawString(21, textY, "LABEL ERROR");
    }
  }
}

void renderEntrySet(std::vector<Control::Control*>* entries, uint8_t selectedTab, uint8_t selectedEntry) {
  if (windowStates[selectedTab].previousSelectedEntry > selectedEntry) {
    if (windowStates[selectedTab].selectedEntryRow == 0) {
      windowStates[selectedTab].entryWindowStart--;
    } else {
      windowStates[selectedTab].selectedEntryRow--;
    }
  } else if (windowStates[selectedTab].previousSelectedEntry < selectedEntry) {
    if (windowStates[selectedTab].selectedEntryRow == 3) {
      windowStates[selectedTab].entryWindowStart++;
    } else {
      windowStates[selectedTab].selectedEntryRow++;
    }
  }
  windowStates[selectedTab].previousSelectedEntry = selectedEntry;
  for (uint8_t i = 0; i < 4; i++) {
    if (i + windowStates[selectedTab].entryWindowStart < entries->size()) {
      renderEntry((*entries)[i + windowStates[selectedTab].entryWindowStart], i);
    }
  }
  renderSelectedEntryBox(windowStates[selectedTab].selectedEntryRow);
  renderScrollBar(entries->size(), windowStates[selectedTab].entryWindowStart);
}

void renderIcon(Icons::StatusIcon* icon, uint8_t row) {
  for (uint8_t x = 0; x < 16; x++) {
    for (uint8_t y = 0; y < 16; y++) {
      if (icon->data[y][x]) {
        display.setPixel(x, y + row * 20);
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

void render(std::vector<Control::Control*>* entries, uint8_t selectedTab, uint8_t selectedEntry, std::list<Icons::StatusIcon*>* icons) {
  display.clear();
  display.setColor(BLACK);
  display.fillRect(0, 0, 128, 64);
  display.setColor(WHITE);

  renderIconSet(icons);
  renderEntrySet(entries, selectedTab, selectedEntry);

  // display.setColor(BLACK);
  // display.fillRect(0, 0, 50, 16);
  // display.setColor(WHITE);

  // uint32_t localTime = RVLGetAnimationClock();
  // uint16_t ms = localTime % 1000;
  // uint8_t s = (localTime / 1000) % 60;
  // uint8_t m = (localTime / 1000 / 60) % 60;
  // char timeString[9];
  // snprintf(timeString, strlen(timeString), "%02d:%02d:%02d", m, s, ms);
  // display.drawString(0, 0, timeString);

  display.display();
}

}  // namespace Render

#endif  // HAS_UI
