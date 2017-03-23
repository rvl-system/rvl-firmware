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

#define BUTTON_NEXT_CONTROL 13
#define BUTTON_NEXT_CONTROL_ON LOW
#define BUTTON_NEXT_CONTROL_OFF HIGH
#define ENCODER_A 5
#define ENCODER_B 4
#define ENCODER_STEPS_PER_CLICK 4

#define CONTROL_PRESS_ENGAGE_TIME 50
#define CONTROL_HOLD_ENGAGE_TIME 2000
#define CONTROL_HOLD_ENGAGE_STEP_INTERVAL 50

#define CLIENT_TIMEOUT 5000

#endif
