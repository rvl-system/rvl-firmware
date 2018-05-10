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

#include <Arduino.h>
#include "./messaging/protocols/giggle_pixel/preset.h"
#include "./messaging/protocols/giggle_pixel/giggle_pixel.h"
#include "./messaging/transport.h"
#include "../../../config.h"  // Why does this one single file require ".." but none of the others do?
#include "./state.h"
#include "./event.h"
#include "./codes.h"

namespace Preset {

uint32 nextSyncTime = millis();

void sync();

void init() {
  Event::on(Codes::EventType::AnimationChange, sync);
}

void loop() {
  if (State::getSettings()->mode != Codes::Mode::Controller) {
    return;
  }
  if (millis() < nextSyncTime) {
    return;
  }
  nextSyncTime = millis() + CLIENT_SYNC_INTERVAL;
  sync();
}

void sync() {
  Serial.println("Syncing raver lights");
  State::Settings* settings = State::getSettings();

  Transport::beginWrite();
  GigglePixel::broadcastHeader(
    Codes::GigglePixelPacketTypes::Preset,
    0,  // Priority
    4 + 1 + 1 + NUM_PRESET_VALUES);
  Transport::write8(settings->preset);
  for (int i = 0; i < NUM_PRESET_VALUES; i++) {
    Transport::write8(settings->presetValues[settings->preset][i]);
  }
  Transport::endWrite();
}

void parsePacket() {
  Serial.println("Parsing Raver Lights packet");
  uint8 preset = Transport::read8();
  uint8 presetValues[NUM_PRESET_VALUES];
  Transport::read(presetValues, NUM_PRESET_VALUES);
  State::setAnimation(preset, presetValues);
}

}  // namespace Preset
