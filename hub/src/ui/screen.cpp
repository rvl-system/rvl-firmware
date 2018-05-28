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
#include "../config.h"
#include "./state.h"
#include "./codes.h"
#include "./event.h"

namespace Screen {

void update() {
  auto settings = State::getSettings();
  std::list<Icons::StatusIcon*> icons;
  if (settings->wifiConnected) {
    icons.push_back(&Icons::wifiConnectedIcon);
  } else {
    icons.push_back(&Icons::wifiDisconnectedIcon);
  }
  Render::render(&(UIState::controls), UIState::currentControl, &icons);
}

void init() {
  Event::on(Codes::EventType::UIStateChange, update);
  Render::init();
  Screen::update();
  Serial.println("Screen initialized");
}

void loop() {
}

}  // namespace Screen
