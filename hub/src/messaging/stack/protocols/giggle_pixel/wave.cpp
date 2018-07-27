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
#include "./messaging/stack/protocols/giggle_pixel/wave.h"
#include "./messaging/stack/protocols/giggle_pixel/giggle_pixel.h"
#include "./messaging/stack/transport.h"
#include "../../../../config.h"  // Why does this one single file require ".." but none of the others do?
#include "./state.h"
#include "./event.h"
#include "./codes.h"
#include "./util/logging.h"

namespace Wave {

uint32 nextSyncTime = millis();

TransportInterface* transport;

void sync();

void init(TransportInterface* newTransport) {
  transport = newTransport;
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
  Logging::debug("Syncing preset");
  auto settings = State::getSettings();
  uint16 length = sizeof(State::Wave) * NUM_WAVES;
  transport->beginWrite();
  GigglePixel::broadcastHeader(
    Codes::GigglePixelPacketTypes::Wave,
    0,  // Priority
    2 + length);
  transport->write8(settings->waveSettings.timePeriod);
  transport->write8(settings->waveSettings.distancePeriod);
  transport->write(reinterpret_cast<uint8*>(&settings->waveSettings.waves), length);
  transport->endWrite();
}

void parsePacket() {
  Logging::debug("Parsing Wave packet");
  State::WaveSettings newWaveSettings;
  newWaveSettings.timePeriod = transport->read8();
  newWaveSettings.distancePeriod = transport->read8();
  transport->read(reinterpret_cast<uint8*>(&newWaveSettings.waves), sizeof(State::Wave) * NUM_WAVES);
  State::setWaveParameters(&newWaveSettings);
}

}  // namespace Wave
