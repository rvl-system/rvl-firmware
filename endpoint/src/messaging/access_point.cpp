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

#ifdef IS_ACCESS_POINT

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "lights.h"
#include "messaging.h"

#define NUM_CLIENTS 1

#define UPDATE_RATE 5000

char* clients[NUM_CLIENTS] = {
  "192.168.4.2"
};

unsigned long last_update;

void access_point_messaging_setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.print("Configuring access point '");
  Serial.print(WIFI_SSID);
  Serial.println("'");
  WiFi.softAP(WIFI_SSID, WIFI_PASSPHRASE);

  IPAddress myIP = WiFi.softAPIP();

  Serial.print("AP IP address: ");
  Serial.println(myIP);
  last_update = millis();
}

void update_preset(unsigned char preset) {
  Serial.print("connecting to ");
  Serial.println(clients[0]);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(clients[0], 80)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  client.write(1);
  client.write(preset);
}

void access_point_messaging_loop() {
  if (millis() - last_update > UPDATE_RATE) {
    last_update = millis();
    update_preset(1);
  }
}

#endif
