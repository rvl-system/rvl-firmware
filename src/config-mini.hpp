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

#ifndef CONFIG_MINI_H_
#define CONFIG_MINI_H_

#define LED_DATA_PIN 33

// The RGB status LED's anodes, active high, one LEDC channel each. Confirmed on
// hardware: the PCB's footprint has red and green the other way around
#define STATUS_LED_R_PIN 22
#define STATUS_LED_G_PIN 23
#define STATUS_LED_B_PIN 21

#endif // CONFIG_MINI_H_
