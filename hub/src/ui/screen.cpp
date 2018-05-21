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

#include <Arduino.h>
#include "./ui/screen.h"
#include "./ui/ui_state.h"
#include "./ui/screen/render.h"
#include "./ui/screen/icons.h"
#include "./ui/controls/control.h"
#include "./ui/controls/base_controls.h"
#include "./ui/controls/preset_controls.h"
#include "../config.h"
#include "./state.h"
#include "./codes.h"
#include "./event.h"

namespace Screen {

void update();

void init() {
  Event::on(Codes::EventType::UIStateChange, update);
  Render::init();
  Screen::update();
  Serial.println("Screen initialized");
}

void loop() {
}

void update() {
  auto settings = State::getSettings();

  std::vector<Control::Control*> entries = {
    &BaseControls::brightnessControl,
    &BaseControls::wifiControl,
    &BaseControls::modeControl,
    &BaseControls::presetControl
  };
  // Uhhh, is this the best way to do this?
  BaseControls::brightnessControl.value = settings->brightness;
  BaseControls::presetControl.selectedValueIndex = settings->presetSettings.preset;

  switch (settings->presetSettings.preset) {
    case Codes::Preset::Rainbow:
      PresetControls::rainbowRateControl.value =
        settings->presetSettings.presetValues[Codes::Preset::Rainbow][0];
      entries.push_back(&PresetControls::rainbowRateControl);

      break;

    case Codes::Preset::Pulse:
      PresetControls::pulseRateControl.value =
        settings->presetSettings.presetValues[Codes::Preset::Pulse][0];
      entries.push_back(&PresetControls::pulseRateControl);

      PresetControls::pulseHueControl.value =
        settings->presetSettings.presetValues[Codes::Preset::Pulse][1];
      entries.push_back(&PresetControls::pulseHueControl);

      PresetControls::pulseSaturationControl.value =
        settings->presetSettings.presetValues[Codes::Preset::Pulse][2];
      entries.push_back(&PresetControls::pulseSaturationControl);

      break;

    case Codes::Preset::Wave:
      PresetControls::waveRateControl.value =
        settings->presetSettings.presetValues[Codes::Preset::Wave][0];
      entries.push_back(&PresetControls::waveRateControl);

      PresetControls::waveForegroundHueControl.value =
        settings->presetSettings.presetValues[Codes::Preset::Wave][1];
      entries.push_back(&PresetControls::waveForegroundHueControl);

      PresetControls::waveForegroundSaturationControl.value =
        settings->presetSettings.presetValues[Codes::Preset::Wave][2];
      entries.push_back(&PresetControls::waveForegroundSaturationControl);

      PresetControls::waveBackgroundHueControl.value =
        settings->presetSettings.presetValues[Codes::Preset::Wave][3];
      entries.push_back(&PresetControls::waveBackgroundHueControl);

      PresetControls::waveBackgroundSaturationControl.value =
        settings->presetSettings.presetValues[Codes::Preset::Wave][4];
      entries.push_back(&PresetControls::waveBackgroundSaturationControl);

      break;
  }

  std::list<Icons::StatusIcon*> icons;
  if (settings->wifiConnected) {
    icons.push_back(&Icons::wifiConnectedIcon);
  } else {
    icons.push_back(&Icons::wifiDisconnectedIcon);
  }

  Render::render(&entries, UIState::currentControl, &icons);
}

}  // namespace Screen
