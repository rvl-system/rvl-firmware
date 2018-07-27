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

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "./messaging/messaging.h"
#include "./messaging/udp_transport.h"
#include "./messaging/stack/stack.h"
#include "../config.h"  // Why does this one single file require ".." but none of the others do?
#include "./codes.h"
#include "./state.h"
#include "./util/logging.h"

namespace Messaging {

#define STATE_DISCONNECTED 0
#define STATE_CONNECTING 1
#define STATE_CONNECTED 2

byte state = STATE_DISCONNECTED;

WiFiUDP udp;
UDPTransport::UDPTransport transport(&udp);

void init() {
  WiFi.setSleepMode(WIFI_NONE_SLEEP);   // Helps keep LEDs from flickering
  Stack::init(&transport);
  Logging::info("Messaging initialized");
}

void loop() {
  switch (state) {
    case STATE_DISCONNECTED:
      Logging::info("Connecting to %s", WIFI_SSID);
      WiFi.begin(WIFI_SSID, WIFI_PASSPHRASE);
      state = STATE_CONNECTING;
      // Fall through here instead of breaking
    case STATE_CONNECTING:
      if (WiFi.status() == WL_CONNECTED) {
        Logging::info("Connected to WiFi with address %s", WiFi.localIP());
        State::setClientId(WiFi.localIP()[3]);
        udp.begin(SERVER_PORT);
        state = STATE_CONNECTED;
        State::setWifiConnected(true);
      }
      break;
    case STATE_CONNECTED:
      if (WiFi.status() != WL_CONNECTED) {
        Logging::info("Disconnected from WiFi, retrying");
        state = STATE_DISCONNECTED;
        State::setWifiConnected(false);
        break;
      }
      int packetSize = udp.parsePacket();
      if (packetSize == 0) {
        return;
      }
      Logging::debug("Packet received of length %d", packetSize);
      Stack::parsePacket();
      break;
  }
}

}  // namespace Messaging
