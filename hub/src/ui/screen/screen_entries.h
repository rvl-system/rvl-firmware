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

#ifndef UI_SCREEN_SCREEN_ENTRIES_H_
#define UI_SCREEN_SCREEN_ENTRIES_H_

#include "./ui/screen/render.h"

namespace ScreenEntries {

extern Render::Entry brightnessEntry;
extern Render::Entry wifiEntry;
extern Render::Entry modeEntry;
extern Render::Entry presetEntry;

extern Render::Entry rainbowRateEntry;

extern Render::Entry pulseRateEntry;
extern Render::Entry pulseHueEntry;
extern Render::Entry pulseSaturationEntry;

extern Render::Entry waveRateEntry;
extern Render::Entry waveForegroundHueEntry;
extern Render::Entry waveForegroundSaturationEntry;
extern Render::Entry waveBackgroundHueEntry;
extern Render::Entry waveBackgroundSaturationEntry;

}  // namespace ScreenEntries

#endif  // UI_SCREEN_SCREEN_ENTRIES_H_
