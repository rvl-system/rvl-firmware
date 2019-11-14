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

#ifndef CODES_H_
#define CODES_H_

namespace Codes {

namespace EventType {
  enum EventType {
    AnimationChange,
    UIStateChange,
    ConnectedStateChange,
    ModeChange,
    BrightnessChange,
    PowerStateChange,
    TimeChange,
    SynchronizationChange
  };
}  // namespace EventType

namespace Preset {
  enum Preset {
    Rainbow = 0,
    Pulse = 1,
    Wave = 2,
    ColorCycle = 3,
    Unknown = 255
  };
}  // namespace Preset

namespace RainbowPresetValues {
  enum RainbowPresetValues {
    Rate = 0
  };
}  // namespace RainbowPresetValues

namespace PulsePresetValues {
  enum PulsePresetValues {
    Rate = 0,
    Hue = 1,
    Saturation = 2
  };
}  // namespace PulsePresetValues

namespace WavePresetValues {
  enum WavePresetValues {
    Rate = 0,
    ForegroundHue = 1,
    ForegroundSaturation = 2,
    BackgroundHue = 3,
    BackgroundSaturation = 4
  };
}  // namespace WavePresetValues

namespace ColorCyclePresetValues {
  enum ColorCyclePresetValues {
    Rate = 0
  };
}  // namespace ColorCyclePresetValues

}  // namespace Codes

#endif  // CODES_H_
