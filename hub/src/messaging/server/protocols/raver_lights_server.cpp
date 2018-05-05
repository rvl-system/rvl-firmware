/*
Copyright (c) 2018 Bryan Hughes <bryan@nebri.us>

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

#include "./messaging/server/protocols/raver_lights_server.h"
#include "./messaging/broadcast.h"
#include "./config.h"
#include "./state.h"
#include "./event.h"

namespace RaverLightsServer {

byte currentPreset = Codes::Preset::Unknown;
uint32 commandStartTime = millis();
uint32 nextSyncTime = millis();
uint8 numConnectedClients = 0;

bool needsSync = false;

void sync();

void update();
class MessagingServerStateListener : public Event::EventListenerInterface {
 public:
  void onEvent() {
    update();
  }
};

void init() {
  Event::on(Codes::EventTypes::AnimationChange, new MessagingServerStateListener());
}

void loop() {
  if (millis() < nextSyncTime && !needsSync) {
    return;
  }
  needsSync = false;

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

void sync() {
  Serial.println("Syncing");
  State::Settings* settings = State::getSettings();
  uint32 commandTime = static_cast<uint32>(millis() - commandStartTime);
  settings->commandTime = commandTime;

  Broadcast::begin();
  Broadcast::write(static_cast<byte*>(static_cast<void*>(&commandTime)), 4);
  Broadcast::write(settings->brightness);
  Broadcast::write(settings->preset);
  for (int i = 0; i < NUM_PRESET_VALUES; i++) {
    Broadcast::write(settings->presetValues[settings->preset][i]);
  }
  Broadcast::end();
}

}  // namespace RaverLightsServer

