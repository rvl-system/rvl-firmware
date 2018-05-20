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

  std::vector<Render::Entry*> entries = {
    &ScreenEntries::brightnessEntry,
    &ScreenEntries::wifiEntry,
    &ScreenEntries::modeEntry,
    &ScreenEntries::presetEntry,
    &ScreenEntries::rateEntry
  };
  Render::EntrySet entrySet = { entries, UIState::currentControl };

  std::list<Render::Icon*> icons = {
    &Icons::wifiConnectedIcon
  };
  Render::IconSet iconSet = { icons };

  Render::render(&entrySet, &iconSet);
}

}  // namespace Screen
