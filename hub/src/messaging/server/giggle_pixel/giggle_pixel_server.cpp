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

#include "./messaging/server/giggle_pixel/giggle_pixel_server.h"
#include "./messaging/server/giggle_pixel/raver_lights_server.h"

namespace GigglePixelServer {

void init() {
  RaverLightsServer::init();
}

void loop() {
  RaverLightsServer::loop();
}

}  // namespace GigglePixelServer
