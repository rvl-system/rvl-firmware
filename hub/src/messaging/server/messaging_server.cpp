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
#include <Arduino.h>
#include "./messaging/server/messaging_server.h"
#include "./messaging/server/clock_sync/clock_sync_server.h"
#include "./messaging/server/giggle_pixel/giggle_pixel_server.h"
#include "./messaging/giggle_pixel.h"
#include "./config.h"
#include "./state.h"


namespace MessagingServer {

void init() {
  Serial.print("Setting soft-AP configuration...");
  if (WiFi.softAPConfig(SERVER_IP, GATEWAY, SUBNET)) {
    Serial.println("Ready");
  } else {
    Serial.println("Failed!");
    return;
  }

  Serial.print("Starting soft-AP ");
  Serial.print(WIFI_SSID);
  Serial.print("...");

  if (WiFi.softAP(WIFI_SSID, WIFI_PASSPHRASE, 8, false)) {
    Serial.println("Ready");
  } else {
    Serial.println("Failed!");
    return;
  }

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  GigglePixel::setClientId(WiFi.softAPIP()[3]);

  ClockSyncServer::init();
  GigglePixelServer::init();

  Serial.println("Messaging initialized");
}

void loop() {
  State::setClientsConnected(WiFi.softAPgetStationNum());
  ClockSyncServer::loop();
  GigglePixelServer::loop();
}

}  // namespace MessagingServer
