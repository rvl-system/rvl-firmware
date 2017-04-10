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

#ifndef EVENTS_HEADER
#define EVENTS_HEADER

namespace Events {

  void emitControlEvent(Codes::Control::Control currentControl);
  void emitValueEvent(Codes::Preset::Preset preset, int code, int newValue);
  void emitBrightnessEvent(int brightness);
  void emitPresetEvent(Codes::Preset::Preset preset);
  void emitClientEvent(int numClients);
  void emitIdleEvent(Codes::IdleState::IdleState idleState);

}

#endif
