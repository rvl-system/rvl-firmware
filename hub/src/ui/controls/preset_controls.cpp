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
#include "./state.h"
#include "./codes.h"
#include "./ui/ui_state.h"
#include "./ui/controls/control.h"
#include "./ui/controls/preset_controls.h"
#include "./ui/presets/rainbow.h"
#include "./ui/presets/pulse.h"
#include "./event.h"

namespace PresetControls {

void increaseValueHelper(uint8 preset, uint8 presetIndex) {
  if (State::getSettings()->mode != Codes::Mode::Controller) {
    return;
  }
  uint8* values = UIState::presetValues[preset];
  if (values[presetIndex] < 255) {
    values[presetIndex]++;
    Event::emit(Codes::EventType::AnimationChange);
  }
}

void decreaseValueHelper(uint8 preset, uint8 presetIndex) {
  if (State::getSettings()->mode != Codes::Mode::Controller) {
    return;
  }
  uint8* values = UIState::presetValues[preset];
  if (values[presetIndex] > 0) {
    values[presetIndex]--;
    Event::emit(Codes::EventType::AnimationChange);
  }
}

uint8 getValueHelper(uint8 preset, uint8 presetIndex) {
  return UIState::presetValues[preset][presetIndex];
}

// Rainbow preset entries

void increaseRainbowRateValue() {
  increaseValueHelper(Codes::Preset::Rainbow, Codes::RainbowPresetValues::Rate);
  Rainbow::calculateWaveParameters();
}
void decreaseRainbowRateValue() {
  decreaseValueHelper(Codes::Preset::Rainbow, Codes::RainbowPresetValues::Rate);
  Rainbow::calculateWaveParameters();
}
uint8 getRainbowRateValue() {
  return getValueHelper(Codes::Preset::Rainbow, Codes::RainbowPresetValues::Rate);
}
Control::RangeControl rainbowRateControl(
  increaseRainbowRateValue,
  decreaseRainbowRateValue,
  getRainbowRateValue,
  "RATE");

// Pulse preset entries

void increasePulseRateValue() {
  increaseValueHelper(Codes::Preset::Pulse, Codes::PulsePresetValues::Rate);
  Pulse::calculateWaveParameters();
}
void decreasePulseRateValue() {
  decreaseValueHelper(Codes::Preset::Pulse, Codes::PulsePresetValues::Rate);
  Pulse::calculateWaveParameters();
}
uint8 getPulseRateValue() {
  return getValueHelper(Codes::Preset::Pulse, Codes::PulsePresetValues::Rate);
}
Control::RangeControl pulseRateControl(
  increasePulseRateValue,
  decreasePulseRateValue,
  getPulseRateValue,
  "RATE");

void increasePulseHueValue() {
  increaseValueHelper(Codes::Preset::Pulse, Codes::PulsePresetValues::Hue);
  Pulse::calculateWaveParameters();
}
void decreasePulseHueValue() {
  decreaseValueHelper(Codes::Preset::Pulse, Codes::PulsePresetValues::Hue);
  Pulse::calculateWaveParameters();
}
uint8 getPulseHueValue() {
  return getValueHelper(Codes::Preset::Pulse, Codes::PulsePresetValues::Hue);
}
Control::RangeControl pulseHueControl(
  increasePulseHueValue,
  decreasePulseHueValue,
  getPulseHueValue,
  "HUE");

void increasePulseSaturationValue() {
  increaseValueHelper(Codes::Preset::Pulse, Codes::PulsePresetValues::Saturation);
  Pulse::calculateWaveParameters();
}
void decreasePulseSaturationValue() {
  decreaseValueHelper(Codes::Preset::Pulse, Codes::PulsePresetValues::Saturation);
  Pulse::calculateWaveParameters();
}
uint8 getPulseSaturationValue() {
  return getValueHelper(Codes::Preset::Pulse, Codes::PulsePresetValues::Saturation);
}
Control::RangeControl pulseSaturationControl(
  increasePulseSaturationValue,
  decreasePulseSaturationValue,
  getPulseSaturationValue,
  "STR");

// Wave preset entries

void increaseWaveRateValue() {
  increaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::Rate);
}
void decreaseWaveRateValue() {
  decreaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::Rate);
}
uint8 getWaveRateValue() {
  return getValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::Rate);
}
Control::RangeControl waveRateControl(
  increaseWaveRateValue,
  decreaseWaveRateValue,
  getWaveRateValue,
  "RATE");

void increaseWaveForegroundHueValue() {
  increaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::ForegroundHue);
}
void decreaseWaveForegroundHueValue() {
  decreaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::ForegroundHue);
}
uint8 getWaveForegroundHueValue() {
  return getValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::ForegroundHue);
}
Control::RangeControl waveForegroundHueControl(
  increaseWaveForegroundHueValue,
  decreaseWaveForegroundHueValue,
  getWaveForegroundHueValue,
  "HUEF");

void increaseWaveForegroundSaturationValue() {
  increaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::ForegroundSaturation);
}
void decreaseWaveForegroundSaturationValue() {
  decreaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::ForegroundSaturation);
}
uint8 getWaveForegroundSaturationValue() {
  return getValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::ForegroundSaturation);
}
Control::RangeControl waveForegroundSaturationControl(
  increaseWaveForegroundSaturationValue,
  decreaseWaveForegroundSaturationValue,
  getWaveForegroundSaturationValue,
  "STRF");

void increaseWaveBackgroundHueValue() {
  increaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::BackgroundHue);
}
void decreaseWaveBackgroundHueValue() {
  decreaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::BackgroundHue);
}
uint8 getWaveBackgroundHueValue() {
  return getValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::BackgroundHue);
}
Control::RangeControl waveBackgroundHueControl(
  increaseWaveBackgroundHueValue,
  decreaseWaveBackgroundHueValue,
  getWaveBackgroundHueValue,
  "HUEB");

void increaseWaveBackgroundSaturationValue() {
  increaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::BackgroundSaturation);
}
void decreaseWaveBackgroundSaturationValue() {
  decreaseValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::BackgroundSaturation);
}
uint8 getWaveBackgroundSaturationValue() {
  return getValueHelper(Codes::Preset::Wave, Codes::WavePresetValues::BackgroundSaturation);
}
Control::RangeControl waveBackgroundSaturationControl(
  increaseWaveBackgroundSaturationValue,
  decreaseWaveBackgroundSaturationValue,
  getWaveBackgroundSaturationValue,
  "STRB");

}  // namespace PresetControls
