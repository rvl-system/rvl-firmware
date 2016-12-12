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
