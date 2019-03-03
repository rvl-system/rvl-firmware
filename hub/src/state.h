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

#ifndef STATE_H_
#define STATE_H_

#include <Arduino.h>
#include "./codes.h"
#include "./config.h"

namespace State {

struct PaletteEntry {
  uint8 fraction;
  uint8 r;
  uint8 g;
  uint8 b;
};

struct PaletteSettings {
  uint8 paletteEntryCount;
  PaletteEntry paletteEntries[MAX_PALETTE_ENTRIES];
};

struct WaveChannel {
  uint8 a = 0;
  uint8 b = 0;
  int8 w_t = 0;
  int8 w_x = 0;
  int8 phi = 0;
};

struct Wave {
  WaveChannel h;
  WaveChannel s;
  WaveChannel v;
  WaveChannel a;
};

struct WaveSettings {
  uint8 timePeriod = 255;
  uint8 distancePeriod = 32;
  Wave waves[NUM_WAVES];
};

struct Settings {
  Codes::GigglePixelPacketTypes::GigglePixelPacketTypes settingType = Codes::GigglePixelPacketTypes::Wave;
  PaletteSettings paletteSettings;
  WaveSettings waveSettings;

  uint8 brightness = DEFAULT_BRIGHTNESS;
  uint8 mode = Codes::Mode::Controller;

  uint16 id = 0;
  uint32 clock = millis();
};

// ---- New stuff

void init();
void loop();

uint32 getAnimationClock();

uint8 getBrightness();
void setBrightness(uint8 brightness);

bool isWifiConnected();
void setWifiConnectedState(bool connected);

// ---- Old stuff

Settings* getSettings();

void setMode(Codes::Mode::Mode mode);
void setClientId(uint16 id);
void setClockOffset(int32 clockOffset);
void setWaveParameters(WaveSettings *waveSettings);

}  // namespace State

#endif  // STATE_H_
