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

#ifndef CONFIG_HUB_H_
#define CONFIG_HUB_H_

#define LED_DATA_PIN 2

#define LCD_ADDRESS 0x3C
#define LCD_SDA 4
#define LCD_SCL 5
#define LCD_WIDTH 128
#define LCD_HEIGHT 64

#define BUTTON_PRESS 12
#define BUTTON_PRESS_ON LOW
#define BUTTON_PRESS_OFF HIGH

#define BUTTON_UP 15
#define BUTTON_UP_ON LOW
#define BUTTON_UP_OFF HIGH

#define BUTTON_DOWN 16
#define BUTTON_DOWN_ON HIGH
#define BUTTON_DOWN_OFF LOW

#define BUTTON_LEFT 14
#define BUTTON_LEFT_ON LOW
#define BUTTON_LEFT_OFF HIGH

#define BUTTON_RIGHT 13
#define BUTTON_RIGHT_ON LOW
#define BUTTON_RIGHT_OFF HIGH

#endif // CONFIG_HUB_H_
