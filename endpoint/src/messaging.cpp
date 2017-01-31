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
#include "lights.h"
#include "messaging.h"

#define STATE_WAITING 1
#define STATE_READING 2

#define SERVER_PORT 3000
byte SERVER_IP[] = { 192, 168, 1, 143 };

#define HEARTBEAT_RATE 1000
#define HEARTBEAT_CODE 126

unsigned char state = STATE_WAITING;
int message_type;

unsigned int heartbeat_count = 0;

WiFiUDP Udp;

void messaging_setup() {
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

  Udp.begin(SERVER_PORT);
  Serial.println("UDP server listening");
}

void messaging_loop() {
  heartbeat_count++;
  if (heartbeat_count >= HEARTBEAT_RATE && state == STATE_WAITING) {
    heartbeat_count = 0;
    Serial.println("Sending heartbeat");
    Serial.println(WiFi.localIP());
    Udp.beginPacket(SERVER_IP, SERVER_PORT);
    Udp.write(HEARTBEAT_CODE);
    Udp.endPacket();
  }
  if (Udp.available() == 0) {
    Udp.parsePacket();
  }
  if (Udp.available() > 0) {
    Serial.print("Available packets: ");
    Serial.print(Udp.available());
    Serial.println();
    switch (state) {
      case STATE_WAITING:
        message_type = Udp.read();
        Serial.print("Message Type: ");
        Serial.println(message_type);
        state = STATE_READING;
        break;
      case STATE_READING:
        switch(message_type) {
          case MESSAGE_SET_PRESET:
            if (Udp.available() >= 1) {
              int preset = Udp.read();
              Serial.print("Changing preset to: ");
              Serial.println(preset);
              lights_set_preset(preset);
              state = STATE_WAITING;
            }
            break;
          case MESSAGE_SET_VALUE:
            if (Udp.available() >= 2) {
              int type = Udp.read();
              int value = Udp.read();
              Serial.print("Changing value ");
              Serial.print(type);
              Serial.print(" to ");
              Serial.println(value);
              lights_set_value(type, value);
              state = STATE_WAITING;
            }
            break;
          case MESSAGE_SET_BRIGHTNESS:
            if (Udp.available() >= 1) {
              int brightness = Udp.read();
              Serial.print("Changing brightness to: ");
              Serial.println(brightness);
              lights_set_brightness(brightness);
              state = STATE_WAITING;
            }
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
