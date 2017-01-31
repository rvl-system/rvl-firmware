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

#ifndef STATE_HEADER
#define STATE_HEADER

#include "codes.h"

namespace State {

  struct FadeValueSettings {
    int rate = 125;
  };

  struct PulseValueSettings {
    int rate = 125;
    int hue = 0;
    int saturation = 255;
  };

  struct Settings {
    Codes::Preset::Preset preset = Codes::Preset::Fade;
    int brightness = 25;
    FadeValueSettings fadeValues;
    PulseValueSettings pulseValues;
    Codes::Control::Control currentControl = Codes::Control::None;
    int numClients = 0;
    Codes::IdleState::IdleState idleState = Codes::IdleState::Active;
  };

  void init();
  void loop();

  Settings* getSettings();

  void nextControl();
  void controlUp();
  void controlDown();

  void clientConnected();
  void clientDisconnected();

  void setActive();
  void setIdling();

}

#endif
