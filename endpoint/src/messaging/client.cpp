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

#ifndef IS_ACCESS_POINT

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "lights.h"
#include "messaging.h"

#define STATE_WAITING 0
#define STATE_READING 1

#define ACCESS_POINT "192.168.4.1"

byte SERVER[] = { 192, 168, 4, 1 };

unsigned char state = STATE_WAITING;
int message_type;

WiFiClient controller_client;

void client_messaging_setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
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

  if (controller_client.connect(SERVER, 3000)) {
    Serial.println("Connected to coordinator");
  }
}

void client_messaging_loop() {
  if (!controller_client.connected()) {
    Serial.println();
    Serial.println("Disconnected from coordinator");
    controller_client.stop();
    while (true) {}
  }
  if (controller_client.available() > 0) {
    switch (state) {
      case STATE_WAITING:
        message_type = controller_client.read();
        Serial.print("Message Type: ");
        Serial.println(message_type);
        state = STATE_READING;
        break;
      case STATE_READING:
        switch(message_type) {
          case MESSAGE_SET_PRESET:
            if (controller_client.available() >= 1) {
              int preset = controller_client.read();
              Serial.print("Changing preset to: ");
              Serial.println(preset);
              lights_set_preset(preset);
              state = STATE_WAITING;
            }
            break;
          case MESSAGE_SET_VALUE:
            if (controller_client.available() >= 2) {
              int type = controller_client.read();
              int value = controller_client.read();
              Serial.print("Changing value ");
              Serial.print(type);
              Serial.print(" to ");
              Serial.println(value);
              lights_set_value(type, value);
              state = STATE_WAITING;
            }
            break;
          case MESSAGE_SET_BRIGHTNESS:
            // TODO
            break;
          default:
            Serial.print("Unknown message type: ");
            Serial.println(message_type);
        }
        break;
      default:
        Serial.print("Unknown state: ");
        Serial.println(state);
    }
  }
}

#endif
