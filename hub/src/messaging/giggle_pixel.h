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

#ifndef MESSAGING_GIGGLE_PIXEL_H_
#define MESSAGING_GIGGLE_PIXEL_H_

#include <Arduino.h>

namespace GigglePixel {

struct GigglePixelHeaderDetails {
  uint8 protocolVersion;
  uint16 length;
  uint8 packetType;
  uint8 priority;
  uint16 sourceId;
};

void setClientId(uint16 id);
void broadcastHeader(uint8 packetType, uint8 priority, uint16 length);
bool readHeader(GigglePixelHeaderDetails& header);

}  // namespace GigglePixel

#endif  // MESSAGING_GIGGLE_PIXEL_H_
