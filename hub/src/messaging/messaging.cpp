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

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include "./messaging/messaging.h"
#include "./server/messaging_server.h"
#include "./client/messaging_client.h"
#include "./codes.h"
#include "./config.h"
#include "./state.h"

namespace Messaging {

void init() {
  // TODO(nebrius): How to handle init here? Need to move WiFi AP into loop
  MessagingServer::init();
}

void loop() {
  switch (State::getSettings()->mode) {
    case Codes::Mode::Controller:
      MessagingServer::loop();
      break;
    case Codes::Mode::Receiver:
      MessagingClient::loop();
      break;
  }
}

}  // namespace Messaging
