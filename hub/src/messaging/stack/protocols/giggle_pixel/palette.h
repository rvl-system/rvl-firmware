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

#ifndef MESSAGING_STACK_PROTOCOLS_GIGGLE_PIXEL_PALETTE_H_
#define MESSAGING_STACK_PROTOCOLS_GIGGLE_PIXEL_PALETTE_H_

#include "./messaging/stack/transport.h"

namespace Palette {

void init(TransportInterface* newTransport);
void loop();

void parsePacket();

}  // namespace Palette

#endif  // MESSAGING_STACK_PROTOCOLS_GIGGLE_PIXEL_PALETTE_H_
