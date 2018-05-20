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
#include <brzo_i2c.h>
#include <SSD1306Brzo.h>
#include "./ui/screen/render.h"
#include "../../config.h"

namespace Render {

SSD1306Brzo display(LCD_ADDRESS, LCD_SDA, LCD_SCL);

uint8 previousSelectedEntry = 0;  // from 0 to numEntries
uint8 entryWindowStart = 0;   // from 0 to numEntries
uint8 selectedEntryRow = 0;  // from 0 to 3

void init() {
  display.init();
#ifdef INVERT_DISPLAY
  display.flipScreenVertically();
#endif
  display.clear();
  display.setFont(ArialMT_Plain_10);
}

void renderScrollBar(uint8 numEntries, uint8 windowStart) {
  if (numEntries > 4) {
    uint8 scrollBarHeight = 24;
    uint8 y = (64 - scrollBarHeight) * windowStart / (numEntries - 4);
    if (y > 64 - scrollBarHeight) {  // Can happen when changing the preset and the last items disappear
      y = 64 - scrollBarHeight;
    }
    display.fillRect(125, y, 3, 24);
  }
}

void renderSelectedEntryBox(uint8 row) {
  display.drawRect(18, row * 16, 105, 15);
}

void renderEntry(Entry* entry, uint8 row) {
  uint8 textY = row * 16 + 1;
  display.drawString(21, textY, entry->label);
  switch (entry->entryType) {
    case Render::EntryType::Enum:
      display.drawString(52, textY, "<");
      display.drawStringMaxWidth(60, textY, 53, entry->listEntry->values[entry->listEntry->selectedValueIndex]);
      display.drawString(115, textY, ">");
      break;
    case Render::EntryType::Range:
      uint8 progress = 100 * entry->rangeEntry->value / 255;
      display.drawProgressBar(52, row * 16 + 3, 67, 8, progress);
      break;
  }
}

void renderEntrySet(EntrySet* entrySet) {
  if (previousSelectedEntry > entrySet->selectedEntry) {
    if (selectedEntryRow == 0) {
      entryWindowStart--;
    } else {
      selectedEntryRow--;
    }
  } else if (previousSelectedEntry < entrySet->selectedEntry) {
    if (selectedEntryRow == 3) {
      entryWindowStart++;
    } else {
      selectedEntryRow++;
    }
  }
  previousSelectedEntry = entrySet->selectedEntry;
  for (uint8 i = 0; i < 4; i++) {
    if (i + entryWindowStart < entrySet->entries.size()) {
      renderEntry(entrySet->entries[i + entryWindowStart], i);
    }
  }
  renderSelectedEntryBox(selectedEntryRow);
  renderScrollBar(entrySet->entries.size(), entryWindowStart);
}

void renderIcon(Icon* icon, uint8 row) {
  for (uint8 x = 0; x < 16; x++) {
    for (uint8 y = 0; y < 16; y++) {
      if (icon->data[y][x]) {
        display.setPixel(x, y + row * 16);
      }
    }
  }
}

void renderIconSet(IconSet* iconSet) {
  uint8 i = 0;
  for (auto& icon : iconSet->icons) {
    renderIcon(icon, i);
    i++;
  }
}

void render(EntrySet* entrySet, IconSet* iconSet) {
  display.clear();
  display.setColor(BLACK);
  display.fillRect(0, 0, 128, 64);
  display.setColor(WHITE);

  renderIconSet(iconSet);
  renderEntrySet(entrySet);

  display.display();
}

}  // namespace Render
