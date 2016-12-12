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
