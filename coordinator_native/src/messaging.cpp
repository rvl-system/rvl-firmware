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
#include "config.h"
#include "messaging.h"

IPAddress WIFI_SERVER_IP(192, 168, 42, 1);
IPAddress WIFI_GATEWAY(192, 168, 42, 255);
IPAddress WIFI_SUBNET(255, 255, 255, 0);

WiFiUDP udp;

struct ClientInfo {
  uint32_t ip = 0;
  int timeout = 0;
};

ClientInfo clients[5];

void Messaging::init() {
  Serial.print("Setting soft-AP configuration...");
  Serial.println(WiFi.softAPConfig(WIFI_SERVER_IP, WIFI_GATEWAY, WIFI_SUBNET) ? "Ready" : "Failed!");

  Serial.print("Starting soft-AP ");
  Serial.print(WIFI_SSID);
  Serial.print("...");
  Serial.println(WiFi.softAP(WIFI_SSID, WIFI_PASSPHRASE, 2, false) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  Serial.println("Starting UDP server");
  udp.begin(SERVER_PORT);

  Serial.println("Messaging initialized");
}

void Messaging::loop() {
  if (udp.available() == 0) {
    udp.parsePacket();
  }
  if (udp.available() > 0) {
    Serial.println("Available");
    IPAddress ip = udp.remoteIP();
    for (int i = 0; i < 5; i++) {
      if (clients[i].ip == (uint32_t)ip) {
        clients[i].timeout = CLIENT_TIMEOUT;
        break;
      }
      if (clients[i].ip == 0) {
        Serial.print("Client connected: ");
        Serial.println(ip);
        clients[i].ip = (uint32_t)ip;
        clients[i].timeout = CLIENT_TIMEOUT;
        break;
      }
    }
  }
  for (int i = 0; i < 5; i++) {
    if (clients[i].ip != 0) {
      clients[i].timeout--;
      if (clients[i].timeout == 0) {
        Serial.print("Client disconnected: ");
        IPAddress ip = clients[i].ip;
        Serial.println(ip);
        clients[i].ip = 0;
        clients[i].timeout = 0;
      }
    }
  }
}
