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

#ifdef IS_ACCESS_POINT

#include "messaging/access_point.h"

void messaging_setup() {
  access_point_messaging_setup();
}

void messaging_loop() {
  access_point_messaging_loop();
}

#else

#include "messaging/client.h"

void messaging_setup() {
  client_messaging_setup();
}

void messaging_loop() {
  client_messaging_loop();
}

#endif
