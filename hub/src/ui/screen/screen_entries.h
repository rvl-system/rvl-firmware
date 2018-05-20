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

#ifndef UI_SCREEN_SCREEN_ENTRIES_H_
#define UI_SCREEN_SCREEN_ENTRIES_H_

#include <Arduino.h>
#include <list>

namespace ScreenEntries {

enum class EntryType {
  Range,
  List,
  Static
};

// TODO(nebrius): Convert const char* to std::string? Depending on memory size?

class Entry {
 public:
  const char* label;
  EntryType type;
};

class ListEntry : public Entry {
 public:
  ListEntry(const char* listLabel, std::vector<const char*> listValues, uint8 listSelectedValueIndex);
  std::vector<const char*> values;
  uint8 selectedValueIndex;
};

class RangeEntry : public Entry {
 public:
  RangeEntry(const char* rangeLabel, uint8 rangeValue);
  uint8 value;
};

extern RangeEntry brightnessEntry;
extern ListEntry wifiEntry;
extern ListEntry modeEntry;
extern ListEntry presetEntry;

extern RangeEntry rainbowRateEntry;

extern RangeEntry pulseRateEntry;
extern RangeEntry pulseHueEntry;
extern RangeEntry pulseSaturationEntry;

extern RangeEntry waveRateEntry;
extern RangeEntry waveForegroundHueEntry;
extern RangeEntry waveForegroundSaturationEntry;
extern RangeEntry waveBackgroundHueEntry;
extern RangeEntry waveBackgroundSaturationEntry;

}  // namespace ScreenEntries

#endif  // UI_SCREEN_SCREEN_ENTRIES_H_
