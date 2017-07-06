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

  uint32_t commandTime;
  udp.read((byte*)&commandTime, 4);
  byte brightness = udp.read();
  byte preset = udp.read();
  byte presetValues[NUM_PRESET_VALUES];
  udp.read(presetValues, NUM_PRESET_VALUES);

  Lights::update(commandTime, brightness, (Codes::Preset::Preset)preset, presetValues);
}
