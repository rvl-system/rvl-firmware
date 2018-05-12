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

namespace Messaging {

#define STATE_DISCONNECTED 0
#define STATE_CONNECTING 1
#define STATE_CONNECTED 2

byte state = STATE_DISCONNECTED;
uint32 nextTimeToPrintDot = 0;

UDPTransport::UDPTransport transport;

void init() {
  Stack::init(&transport);
  Serial.println("Messaging Client initialized");
}

void loop() {
  switch (state) {
    case STATE_DISCONNECTED:
      Serial.print("Connecting to ");
      Serial.print(WIFI_SSID);
      WiFi.begin(WIFI_SSID, WIFI_PASSPHRASE);
      state = STATE_CONNECTING;
      nextTimeToPrintDot = millis() + 500;
      // Fall through here instead of breaking
    case STATE_CONNECTING:
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

        State::setClientId(WiFi.localIP()[3]);

        Serial.println("Starting UDP server");
        udp.begin(SERVER_PORT);
        state = STATE_CONNECTED;
      } else if (millis() >= nextTimeToPrintDot) {
        Serial.print(".");
        nextTimeToPrintDot = millis() + 500;
      }
      break;
    case STATE_CONNECTED:
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Disconnected from WiFi, retrying");
        state = STATE_DISCONNECTED;
        break;
      }
      int packetSize = udp.parsePacket();
      if (packetSize == 0) {
        return;
      }
      Serial.println("Packet received");
      Stack::parsePacket();
      break;
  }
}

}  // namespace Messaging
