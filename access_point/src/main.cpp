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

#include <WiFi.h>
#include "../../firmware/src/config.h"

#define PROTOCOL_VERSION 1
#define CLIENT_SYNC_INTERVAL 2000
#define NUM_REFERENCE_BROADCASTS 3
#define REFERENCE_BROADCAST_DELAY 200

uint8_t signature[4] = { 'R', 'V', 'L', 'X' };

WiFiUDP udp;

uint8_t nodeId = 0;
uint32_t nextSyncTime = 0;
uint16_t syncId = 1;

void beginWrite() {
  IPAddress ip(255, 255, 255, 255);
  udp.beginPacket(ip, SERVER_PORT);
}

void write8(uint8_t data) {
  udp.write(data);
}

void write16(uint16_t data) {
  udp.write(data >> 8);
  udp.write(data & 0xFF);
}

void write32(uint32_t data) {
  udp.write(data >> 24);
  udp.write(data >> 16 & 0xFF);
  udp.write(data >> 8 & 0xFF);
  udp.write(data & 0xFF);
}

void write(uint8_t* data, uint16_t length) {
  udp.write(data, length);
}

void endWrite() {
  udp.endPacket();
}

void setup() {
  Serial.begin(115200);

  // Create the access point
  Serial.print("Setting AP with SSID ");
  Serial.println(WIFI_SSID);
  WiFi.softAP(WIFI_SSID, WIFI_PASSPHRASE);

  // Print the IP address of this AP
  IPAddress address = WiFi.softAPIP();
  nodeId = address[3];
  Serial.print("AP IP address is ");
  Serial.println(address);
  nextSyncTime = millis() + CLIENT_SYNC_INTERVAL;
}

void loop() {
}
