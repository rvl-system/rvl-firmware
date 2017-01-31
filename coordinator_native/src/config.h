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
#define LCD_SDA 12
#define LCD_SCL 13
#define LCD_WIDTH 128
#define LCD_HEIGHT 64

#define ACCELEROMETER_ADDRESS 0x3C
#define ACCELEROMETER_SDA 16
#define ACCELEROMETER_SCL 14

#define BUTTON_NEXT_CONTROL 15
#define BUTTON_CONTROL_UP 4
#define BUTTON_CONTROL_DOWN 5

#define CONTROL_PRESS_ENGAGE_TIME 50
#define CONTROL_HOLD_ENGAGE_TIME 2000
#define CONTROL_HOLD_ENGAGE_STEP_INTERVAL 50

#endif
