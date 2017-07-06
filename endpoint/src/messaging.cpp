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
#include "config.h"
#include "common/codes.h"
#include "lights.h"
#include "messaging.h"

WiFiUDP udp;

void Messaging::setup() {
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  Serial.println("...");

  WiFi.begin(WIFI_SSID, WIFI_PASSPHRASE);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP server");
  udp.begin(SERVER_PORT);

  Serial.println("Messaging initialized");
}

void Messaging::loop() {
  int packetSize = udp.parsePacket();
  if (packetSize == 0) {
    return;
  }
  Serial.print("Sync packet received");
  if (packetSize != 16) {
    Serial.println("Received incorrect packet size, resetting...");
    ESP.reset();
  }

  unsigned long commandTime;
  udp.read((byte*)&commandTime, 4);
  Lights::setCommandTime(commandTime);

  Lights::setBrightness(udp.read());

  Lights::setPreset((Codes::Preset::Preset)udp.read());

  Lights::setValue((byte)0, udp.read());
  // Lights::setValue((byte)1, udp.read());
  // Lights::setValue((byte)2, udp.read());

  for (int i = 0; i < 7; i++) {
    udp.read();
  }
}
