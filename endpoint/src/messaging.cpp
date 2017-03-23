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

#define STATE_WAITING 1
#define STATE_READING 2

IPAddress SERVER_IP(192, 168, 4, 1);

#define HEARTBEAT_RATE 1000

unsigned char state = STATE_WAITING;
Codes::MessageType::MessageType messageType;

unsigned int heartbeatCount = 0;

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

  udp.begin(SERVER_PORT);
  Serial.println("UDP server listening");

  Serial.println("Messaging initialized");
}

void Messaging::loop() {
  heartbeatCount++;
  if (heartbeatCount >= HEARTBEAT_RATE && state == STATE_WAITING) {
    heartbeatCount = 0;
    Serial.println("Sending heartbeat");
    Serial.println(WiFi.localIP());
    udp.beginPacket(SERVER_IP, SERVER_PORT);
    udp.write(HEARTBEAT_CODE);
    udp.endPacket();
  }
  if (udp.available() == 0) {
    udp.parsePacket();
  }
  if (udp.available() > 0) {
    Serial.print("Available packets: ");
    Serial.print(udp.available());
    Serial.println();
    switch (state) {
      case STATE_WAITING:
        switch(udp.read()) {
          case Codes::MessageType::SetPreset:
            messageType = Codes::MessageType::SetPreset;
            break;
          case Codes::MessageType::SetValue:
            messageType = Codes::MessageType::SetValue;
            break;
          case Codes::MessageType::SetBrightness:
            messageType = Codes::MessageType::SetBrightness;
            break;
          default:
            Serial.print("Received unkown message type");
            return;
        }
        Serial.println(messageType);
        state = STATE_READING;
        break;
      case STATE_READING:
        switch(messageType) {
          case Codes::MessageType::SetPreset:
            if (udp.available() >= 1) {
              Codes::Preset::Preset preset;
              switch(udp.read()) {
                case Codes::Preset::Fade:
                  preset = Codes::Preset::Fade;
                  break;
                case Codes::Preset::Pulse:
                  preset = Codes::Preset::Pulse;
                  break;
                default:
                  Serial.print("Received unkown preset");
                  state = STATE_WAITING;
                  return;
              }
              Serial.print("Changing preset to: ");
              Serial.println(preset);
              Lights::setPreset(preset);
              state = STATE_WAITING;
            }
            break;
          case Codes::MessageType::SetValue:
            if (udp.available() >= 2) {
              int type = udp.read();
              int value = udp.read();
              Serial.print("Changing value ");
              Serial.print(type);
              Serial.print(" to ");
              Serial.println(value);
              Lights::setValue(type, value);
              state = STATE_WAITING;
            }
            break;
          case Codes::MessageType::SetBrightness:
            if (udp.available() >= 1) {
              int brightness = udp.read();
              Serial.print("Changing brightness to: ");
              Serial.println(brightness);
              Lights::setBrightness(brightness);
              state = STATE_WAITING;
            }
            break;
        }
        break;
      default:
        Serial.print("Unknown state: ");
        Serial.println(state);
    }
  }
}
