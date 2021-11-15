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

#ifdef HAS_UI

#include "./ui/screen.hpp"
#include "../config.hpp"
#include "./codes.hpp"
#include "./state.hpp"
#include "./ui/screen/icons.hpp"
#include "./ui/screen/render.hpp"
#include "./ui/ui_state.hpp"
#include <Arduino.h>

namespace Screen {

void update() {
  if (!UIState::isScreenActive()) {
    Render::off();
    return;
  }
  std::list<Icons::StatusIcon*> icons;
  if (rvl::isNetworkConnected()) {
    icons.push_back(&Icons::wifiConnectedIcon);
  } else {
    icons.push_back(&Icons::wifiDisconnectedIcon);
  }
  if (rvl::getSynchronizationState()) {
    icons.push_back(&Icons::synchronizedIcon);
  } else {
    icons.push_back(&Icons::notSynchronizedIcon);
  }
  if (UIState::currentTab == 0) {
    icons.push_back(&Icons::tab1);
    Render::render(&(UIState::tab1Controls), UIState::currentTab,
        UIState::currentTab1Control, &icons);
  } else {
    icons.push_back(&Icons::tab2);
    Render::render(&(UIState::tab2Controls), UIState::currentTab,
        UIState::currentTab2Control, &icons);
  }
}

void init() {
  rvl::on(Codes::EventType::UIStateChange, update);
  rvl::on(EVENT_BRIGHTNESS_UPDATED, update);
  Render::init();
  Screen::update();
}

void loop() {
  Screen::update();
}

} // namespace Screen

#endif // HAS_UI
