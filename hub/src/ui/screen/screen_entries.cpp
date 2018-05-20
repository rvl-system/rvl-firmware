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

#include <vector>
#include "./ui/screen/screen_entries.h"
#include "../../config.h"

namespace ScreenEntries {

ListEntry::ListEntry(const char* listLabel, std::vector<const char*> listValues, uint8 listSelectedValueIndex) {
  this->type = EntryType::List;
  this->label = listLabel;
  this->values = listValues;
  this->selectedValueIndex = listSelectedValueIndex;
}

RangeEntry::RangeEntry(const char* rangeLabel, uint8 rangeValue) {
  this->type = EntryType::Range;
  this->label = rangeLabel;
  this->value = rangeValue;
}

RangeEntry brightnessEntry("BRT", 0);
ListEntry wifiEntry("WIFI", { "RVL1", "RVL2", "RVL3", "RVL4" }, 0);
ListEntry modeEntry("MODE", { "Controller", "Receiver" }, 0);
ListEntry presetEntry("PRST", { "Rainbow", "Pulse", "Wave" }, 0);

// Rainbow preset entries

RangeEntry rainbowRateEntry("RATE", 0);

// Pulse preset entries

RangeEntry pulseRateEntry("RATE", 0);
RangeEntry pulseHueEntry("HUE", 0);
RangeEntry pulseSaturationEntry("STR", 0);

// Wave preset entries

RangeEntry waveRateEntry("RATE", 0);
RangeEntry waveForegroundHueEntry("HUEF", 0);
RangeEntry waveForegroundSaturationEntry("STRF", 0);
RangeEntry waveBackgroundHueEntry("HUEB", 0);
RangeEntry waveBackgroundSaturationEntry("STRB", 0);

}  // namespace ScreenEntries
