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
#include "./config.h"
#include "./messaging/messaging_server.h"
#include "./state.h"
#include "./events.h"

namespace MessagingServer {

byte currentPreset = Codes::Preset::Unknown;
uint32 commandStartTime = millis();

bool needsSync = false;

void sync();

class MessagingServerStateListener : public Events::EventListenerInterface {
 public:
  void onEvent() {
    update();
  }
};

void init() {
  Events::on(Codes::EventTypes::AnimationChange, new MessagingServerStateListener());

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

  Serial.println("Messaging initialized");
}

void sync() {
  Serial.println("Syncing");
  State::Settings* settings = State::getSettings();
  udp.beginPacket(GATEWAY, SERVER_PORT);
  uint32 commandTime = static_cast<uint32>(millis() - commandStartTime);
  State::getSettings()->commandTime = commandTime;
  udp.write(static_cast<byte*>(static_cast<void*>(&commandTime)), 4);
  udp.write(settings->brightness);
  udp.write(settings->preset);
  for (int i = 0; i < NUM_PRESET_VALUES; i++) {
    udp.write(settings->presetValues[settings->preset][i]);
  }
  udp.endPacket();
}

int nextSyncTime = millis();
int numConnectedClients = 0;
void loop() {
  if (millis() < nextSyncTime && !needsSync) {
    return;
  }
  needsSync = false;

  State::setClientsConnected(WiFi.softAPgetStationNum());
  nextSyncTime = millis() + CLIENT_SYNC_INTERVAL;
  sync();
}

void update() {
  byte newPreset = State::getSettings()->preset;
  if (newPreset != currentPreset) {
    currentPreset = newPreset;
    commandStartTime = millis();
  }
  needsSync = true;
}

}  // namespace MessagingServer
