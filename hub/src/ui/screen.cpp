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
#include "../config.h"
#include "./state.h"
#include "./codes.h"
#include "./event.h"

namespace Screen {

Render::Icon wifiConnectedIcon = {
  {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  }
};

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

  Render::EnumEntry e1Enum = {
    { "Option 1", "Option 2" },
    0
  };
  Render::Entry e1 = {
    "One:",
    Render::EntryType::Enum,
    &e1Enum,
    NULL
  };

  Render::EnumEntry e2Enum = {
    { "Option 1", "Option 2" },
    1
  };
  Render::Entry e2 = {
    "Two:",
    Render::EntryType::Enum,
    &e1Enum,
    NULL
  };

  Render::RangeEntry e3Range = { 128 };
  Render::Entry e3 = {
    "Three:",
    Render::EntryType::Range,
    NULL,
    &e3Range
  };

  Render::RangeEntry e4Range = { 50 };
  Render::Entry e4 = {
    "Four:",
    Render::EntryType::Range,
    NULL,
    &e4Range
  };

  Render::RangeEntry e5Range = { 200 };
  Render::Entry e5 = {
    "Five:",
    Render::EntryType::Range,
    NULL,
    &e5Range
  };

  Render::RangeEntry e6Range = { 0 };
  Render::Entry e6 = {
    "Six:",
    Render::EntryType::Range,
    NULL,
    &e6Range
  };

  std::vector<Render::Entry*> entries = {
    &e1, &e2, &e3, &e4, &e5, &e6
  };
  Render::EntrySet entrySet = { entries, UIState::currentControl };

  std::list<Render::Icon*> icons = {
    &wifiConnectedIcon
  };
  Render::IconSet iconSet = { icons };

  Render::render(entrySet, iconSet);
}

}  // namespace Screen
