/*
Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>

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

#ifndef LIGHTS_HEADER
#define LIGHTS_HEADER

#define FADE_PRESET 0
#define PULSE_PRESET 1

void lights_setup();
void lights_loop();
void lights_set_preset(unsigned char new_preset);
void lights_set_value(unsigned char type, unsigned char value);
void lights_set_brightness(unsigned char brightness);

#endif
