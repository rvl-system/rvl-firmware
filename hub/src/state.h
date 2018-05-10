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

struct PresetSettings {
  uint8 preset = DEFAULT_PRESET;
  uint8** presetValues;
};

struct Settings {
  Codes::GigglePixelPacketTypes::GigglePixelPacketTypes settingType = Codes::GigglePixelPacketTypes::Preset;
  PaletteSettings paletteSettings;
  PresetSettings presetSettings;

  uint8 brightness = DEFAULT_BRIGHTNESS;

#ifdef SERVER
  uint8 mode = 0;
#else
  uint8 mode = 1;
#endif

  uint8 currentControl = 0;

  uint16 id = 0;
  uint32 clock = millis();
};

void init();
void loop();

Settings* getSettings();

void nextControl();
void controlUp();
void controlDown();

void setClientId(uint16 id);
void setClockOffset(int32 clockOffset);
void setAnimation(uint8 preset, uint8* presetValues);

}  // namespace State

#endif  // STATE_H_
