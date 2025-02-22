/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

This file is part of RVL Firmware.

RVL Firmware is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RVL Firmware is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RVL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UI_SCREEN_ICONS_H_
#define UI_SCREEN_ICONS_H_

#include <Arduino.h>

#define STATUS_ICON_SIZE 16

namespace Icons {

struct StatusIcon {
  uint8_t data[16][16];
};

extern StatusIcon wifiConnectedIcon;
extern StatusIcon wifiDisconnectedIcon;

extern StatusIcon synchronizedIcon;
extern StatusIcon notSynchronizedIcon;

extern StatusIcon tab1;
extern StatusIcon tab2;

}  // namespace Icons

#endif  // UI_SCREEN_ICONS_H_
