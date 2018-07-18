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

#include <Arduino.h>
#include "./messaging/stack/protocols/giggle_pixel/palette.h"
#include "./messaging/stack/protocols/giggle_pixel/giggle_pixel.h"
#include "./messaging/stack/transport.h"
#include "../../../../config.h"  // Why does this one single file require ".." but none of the others do?
#include "./state.h"
#include "./event.h"
#include "./codes.h"

namespace Palette {

TransportInterface* transport;

void init(TransportInterface* newTransport) {
  transport = newTransport;
}

void loop() {
  // Nothing to do
}

void parsePacket() {
  // TODO(nebrius)
}

}  // namespace Palette
