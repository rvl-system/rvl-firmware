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
#include "./ui/screen.h"
#include "./ui/ui_state.h"
#include "./ui/screen/render.h"
#include "./ui/screen/icons.h"
#include "./ui/screen/screen_entries.h"
#include "../config.h"
#include "./state.h"
#include "./codes.h"
#include "./event.h"

namespace Screen {

void update();

void init() {
  Event::on(Codes::EventType::UIStateChange, update);
  Render::init();
  Screen::update();
  Serial.println("Screen initialized");
}

void loop() {
}

void update() {
  Serial.println("Updating screen");
  auto settings = State::getSettings();

  std::vector<Render::Entry*> entries = {
    &ScreenEntries::brightnessEntry,
    &ScreenEntries::wifiEntry,
    &ScreenEntries::modeEntry,
    &ScreenEntries::presetEntry
  };
  entries[0]->rangeEntry->value = settings->brightness;
  entries[3]->listEntry->selectedValueIndex = settings->presetSettings.preset;
  switch (settings->presetSettings.preset) {
    case Codes::Preset::Rainbow:
      ScreenEntries::rainbowRateEntry.rangeEntry->value = settings->presetSettings.presetValues[Codes::Preset::Rainbow][0];
      entries.push_back(&ScreenEntries::rainbowRateEntry);
      break;
    case Codes::Preset::Pulse:
      ScreenEntries::pulseRateEntry.rangeEntry->value = settings->presetSettings.presetValues[Codes::Preset::Pulse][0];
      entries.push_back(&ScreenEntries::pulseRateEntry);
      break;
    case Codes::Preset::Wave:
      ScreenEntries::waveRateEntry.rangeEntry->value = settings->presetSettings.presetValues[Codes::Preset::Wave][0];
      entries.push_back(&ScreenEntries::waveRateEntry);
      break;
  }
  Render::EntrySet entrySet = { entries, UIState::currentControl };

  std::list<Render::Icon*> icons = {
    &Icons::wifiConnectedIcon
  };
  Render::IconSet iconSet = { icons };

  Render::render(&entrySet, &iconSet);
}

}  // namespace Screen
