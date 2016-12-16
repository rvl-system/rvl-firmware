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
#include "messaging.h"
#include "config.h"

#define STATE_IDLE = 0;
#define STATE_CONNECTING = 1;

char* clientAddresses[] = { "192.168.1.1" };
char* messageQueue[MESSAGE_QUEUE_SIZE];
int messageQueueHead = 0;
int messageQueueTail = 0;
int state = STATE_IDLE;
int currentClient = 0;

WiFiClient client;

void messaging_setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(WIFI_SSID, WIFI_PASSPHRASE);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void messaging_loop() {
  switch (state) {
    case STATE_IDLE:
      if (messageQueueHead != messageQueueTail) {
        state = STATE_CONNECTING;
        currentClient = 0;
        client.connect
      }
      break;
  }
}

void send_message(char* data, size_t length) {
  messageQueue[messageQueueHead++] = data;
  if (messageQueueHead == MESSAGE_QUEUE_SIZE) {
    messageQueueHead = 0;
  }
}
