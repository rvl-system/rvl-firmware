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
#include <vector>
#include "./ui/controls/control.h"
#include "./ui/controls/preset_controls.h"

namespace PresetControls {

// Rainbow preset entries

void increaseRainbowRateValue() {
  // TODO(nebrius): implement me
}
void decreaseRainbowRateValue() {
  // TODO(nebrius): implement me
}
uint8 getRainbowRateValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::RangeControl rainbowRateControl(
  increaseRainbowRateValue,
  decreaseRainbowRateValue,
  getRainbowRateValue,
  "RATE");

// Pulse preset entries

void increasePulseRateValue() {
  // TODO(nebrius): implement me
}
void decreasePulseRateValue() {
  // TODO(nebrius): implement me
}
uint8 getPulseRateValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::RangeControl pulseRateControl(
  increasePulseRateValue,
  decreasePulseRateValue,
  getPulseRateValue,
  "RATE");

void increasePulseHueValue() {
  // TODO(nebrius): implement me
}
void decreasePulseHueValue() {
  // TODO(nebrius): implement me
}
uint8 getPulseHueValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::RangeControl pulseHueControl(
  increasePulseHueValue,
  decreasePulseHueValue,
  getPulseHueValue,
  "HUE");

void increasePulseSaturationValue() {
  // TODO(nebrius): implement me
}
void decreasePulseSaturationValue() {
  // TODO(nebrius): implement me
}
uint8 getPulseSaturationValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::RangeControl pulseSaturationControl(
  increasePulseSaturationValue,
  decreasePulseSaturationValue,
  getPulseSaturationValue,
  "STR");

// Wave preset entries

void increaseWaveRateValue() {
  // TODO(nebrius): implement me
}
void decreaseWaveRateValue() {
  // TODO(nebrius): implement me
}
uint8 getWaveRateValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::RangeControl waveRateControl(
  increaseWaveRateValue,
  decreaseWaveRateValue,
  getWaveRateValue,
  "RATE");

void increaseWaveForegroundHueValue() {
  // TODO(nebrius): implement me
}
void decreaseWaveForegroundHueValue() {
  // TODO(nebrius): implement me
}
uint8 getWaveForegroundHueValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::RangeControl waveForegroundHueControl(
  increaseWaveForegroundHueValue,
  decreaseWaveForegroundHueValue,
  getWaveForegroundHueValue,
  "HUEF");

void increaseWaveForegroundSaturationValue() {
  // TODO(nebrius): implement me
}
void decreaseWaveForegroundSaturationValue() {
  // TODO(nebrius): implement me
}
uint8 getWaveForegroundSaturationValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::RangeControl waveForegroundSaturationControl(
  increaseWaveForegroundSaturationValue,
  decreaseWaveForegroundSaturationValue,
  getWaveForegroundSaturationValue,
  "STRF");

void increaseWaveBackgroundHueValue() {
  // TODO(nebrius): implement me
}
void decreaseWaveBackgroundHueValue() {
  // TODO(nebrius): implement me
}
uint8 getWaveBackgroundHueValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::RangeControl waveBackgroundHueControl(
  increaseWaveBackgroundHueValue,
  decreaseWaveBackgroundHueValue,
  getWaveBackgroundHueValue,
  "HUEB");

void increaseWaveBackgroundSaturationValue() {
  // TODO(nebrius): implement me
}
void decreaseWaveBackgroundSaturationValue() {
  // TODO(nebrius): implement me
}
uint8 getWaveBackgroundSaturationValue() {
  // TODO(nebrius): implement me
  return 0;
}
Control::RangeControl waveBackgroundSaturationControl(
  increaseWaveBackgroundSaturationValue,
  decreaseWaveBackgroundSaturationValue,
  getWaveBackgroundSaturationValue,
  "STRB");

}  // namespace PresetControls
