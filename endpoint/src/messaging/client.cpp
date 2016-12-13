/*
Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
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
