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

#ifndef EVENTS_H_
#define EVENTS_H_

namespace Events {

void emitControlEvent(byte currentControl);
void emitValueEvent(byte preset, byte code, byte newValue);
void emitBrightnessEvent(byte brightness);
void emitPresetEvent(byte preset);
void emitClientEvent(byte numClients);
void emitIdleEvent(Codes::IdleState::IdleState idleState);

}  // namespace Events

#endif  // EVENTS_H_
