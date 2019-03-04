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
#include <RaverLightsMessaging.h>
#include "./messaging.h"
#include "./config.h"
#include "./arduino_platform.h"
#include "./codes.h"
#include "./state.h"
#include "./logging.h"

namespace Messaging {

#define STATE_DISCONNECTED 0
#define STATE_CONNECTING 1
#define STATE_CONNECTED 2

byte state = STATE_DISCONNECTED;

WiFiUDP udp;
UDPTransport transport;

void init() {
  WiFi.setSleepMode(WIFI_NONE_SLEEP);   // Helps keep LEDs from flickering
  RVLMessagingInit(&ArduinoPlatform::platform, &transport, &ArduinoPlatform::logging);
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
        ArduinoPlatform::platform.setDeviceId(WiFi.localIP()[3]);
        udp.begin(SERVER_PORT);
        state = STATE_CONNECTED;
        State::setWifiConnectedState(true);
      }
      break;
    case STATE_CONNECTED:
      if (WiFi.status() != WL_CONNECTED) {
        Logging::info("Disconnected from WiFi, retrying");
        state = STATE_DISCONNECTED;
        State::setWifiConnectedState(false);
        break;
      }
      RVLMessagingLoop();
      break;
  }
}

void UDPTransport::beginWrite() {
  udp.beginPacket(GATEWAY, SERVER_PORT);
}

void UDPTransport::write8(uint8_t data) {
  udp.write(data);
}

void UDPTransport::write16(uint16_t data) {
  udp.write(data >> 8);
  udp.write(data & 0xFF);
}

void UDPTransport::write32(uint32_t data) {
  udp.write(data >> 24);
  udp.write(data >> 16 & 0xFF);
  udp.write(data >> 8 & 0xFF);
  udp.write(data & 0xFF);
}

void UDPTransport::write(uint8_t* data, uint16_t length) {
  udp.write(data, length);
}

void UDPTransport::endWrite() {
  udp.endPacket();
}

uint16_t UDPTransport::parsePacket() {
  return udp.parsePacket();
}

uint8_t UDPTransport::read8() {
  return udp.read();
}

uint16_t UDPTransport::read16() {
  uint16_t val = 0;
  val |= udp.read() << 8;
  val |= udp.read();
  return val;
}

uint32_t UDPTransport::read32() {
  uint32_t val = 0;
  val |= udp.read() << 24;
  val |= udp.read() << 16;
  val |= udp.read() << 8;
  val |= udp.read();
  return val;
}

void UDPTransport::read(uint8_t* buffer, uint16_t length) {
  udp.read(buffer, length);
}

}  // namespace Messaging
