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

#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include "common/common_config.h"

#define LCD_ADDRESS 0x3C
#define LCD_SDA 4
#define LCD_SCL 5
#define LCD_WIDTH 128
#define LCD_HEIGHT 64

#define BUTTON_ON LOW
#define BUTTON_OFF HIGH
#define BUTTON_NEXT 16
#define BUTTON_UP 12
#define BUTTON_DOWN 14

#define BUTTON_PRESS_ENGAGE_TIME 5
#define BUTTON_HOLD_ENGAGE_TIME 200

#define CLIENT_TIMEOUT 5000

#define CLIENT_SYNC_INTERVAL 2000

#endif
