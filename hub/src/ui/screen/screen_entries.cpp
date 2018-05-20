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

#include "./ui/screen/screen_entries.h"
#include "./ui/screen/render.h"
#include "../../config.h"

namespace ScreenEntries {

Render::RangeEntry brightnessRange = { 0 };
Render::Entry brightnessEntry = {
  "BRIT",
  Render::EntryType::Range,
  NULL,
  &brightnessRange
};

Render::EnumEntry wifiEnum = {
  { "RVL1", "RVL2", "RVL3", "RVL4" },
  0
};
Render::Entry wifiEntry = {
  "WIFI",
  Render::EntryType::Enum,
  &wifiEnum,
  NULL
};

Render::EnumEntry modeEnum = {
  { "Controller", "Receiver" },
  0
};
Render::Entry modeEntry = {
  "MODE",
  Render::EntryType::Enum,
  &modeEnum,
  NULL
};

Render::EnumEntry presetEnum = {
  { "Rainbow", "Pulse", "Wave" },
  0
};
Render::Entry presetEntry = {
  "PRST",
  Render::EntryType::Enum,
  &presetEnum,
  NULL
};

Render::RangeEntry rateRange = { 0 };
Render::Entry rateEntry = {
  "RATE",
  Render::EntryType::Range,
  NULL,
  &rateRange
};

// Pulse preset entries

Render::RangeEntry pulseHueRange = { 0 };
Render::Entry pulseHueEntry = {
  "HUE",
  Render::EntryType::Range,
  NULL,
  &pulseHueRange
};

Render::RangeEntry pulseSaturationRange = { 0 };
Render::Entry pulseSaturationEntry = {
  "SATR",
  Render::EntryType::Range,
  NULL,
  &pulseSaturationRange
};

// Fade preset entries

Render::RangeEntry waveForegroundHueRange = { 0 };
Render::Entry waveForegroundHueEntry = {
  "HUEF",
  Render::EntryType::Range,
  NULL,
  &waveForegroundHueRange
};

Render::RangeEntry waveForegroundSaturationRange = { 0 };
Render::Entry waveForegroundSaturationEntry = {
  "SATF",
  Render::EntryType::Range,
  NULL,
  &waveForegroundSaturationRange
};

Render::RangeEntry waveBackgroundHueRange = { 0 };
Render::Entry waveBackgroundHueEntry = {
  "HUEB",
  Render::EntryType::Range,
  NULL,
  &waveBackgroundHueRange
};

Render::RangeEntry waveBackgroundSaturationRange = { 0 };
Render::Entry waveBackgroundSaturationEntry = {
  "SATB",
  Render::EntryType::Range,
  NULL,
  &waveBackgroundSaturationRange
};

}  // namespace ScreenEntries
