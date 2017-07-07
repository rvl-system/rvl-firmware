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
#include "state.h"
#include "common/codes.h"
#include "events.h"

const int BRIGHTNESS_STEP = 1;
const int MAX_BRIGHTNESS = 255;
const int VALUE_STEP = 1;
const int MAX_VALUE = 255;
const int DIM_TIMEOUT = 10000;
const int OFF_TIMEOUT = 5000;

bool isActive = false;
int idleCountdown = DIM_TIMEOUT;

State::Settings settings;

State::Settings* State::getSettings() {
  return &settings;
}

void State::nextControl() {
  int numPresetValues;
  switch (settings.preset) {
    case Codes::Preset::Fade:
      switch(settings.currentControl) {
        case Codes::Control::None:
          settings.currentControl = Codes::Control::Brightness;
          break;
        case Codes::Control::Brightness:
          settings.currentControl = Codes::Control::Preset;
          break;
        case Codes::Control::Preset:
          settings.currentControl = Codes::Control::Value1;
          break;
        case Codes::Control::Value1:
          settings.currentControl = Codes::Control::None;
          break;
      }
      break;
    case Codes::Preset::Pulse:
      switch(settings.currentControl) {
        case Codes::Control::None:
          settings.currentControl = Codes::Control::Brightness;
          break;
        case Codes::Control::Brightness:
          settings.currentControl = Codes::Control::Preset;
          break;
        case Codes::Control::Preset:
          settings.currentControl = Codes::Control::Value1;
          break;
        case Codes::Control::Value1:
          settings.currentControl = Codes::Control::Value2;
          break;
        case Codes::Control::Value2:
          settings.currentControl = Codes::Control::Value3;
          break;
        case Codes::Control::Value3:
          settings.currentControl = Codes::Control::None;
          break;
      }
      break;
  }

  Events::emitControlEvent(settings.currentControl);
}

int calculateNewValue(int value, bool direction) {
  if (direction) {
    value += VALUE_STEP;
    if (value > MAX_VALUE) {
      value = MAX_VALUE;
    }
    return value;
  } else {
    value -= VALUE_STEP;
    if (value < 0) {
      value = 0;
    }
    return value;
  }
}

void handleValueChange(int code, bool direction) {
  int newValue;
  switch(settings.preset) {
    case Codes::Preset::Fade:
      switch(code) {
        case Codes::FadeValue::Rate:
          newValue = settings.fadeValues.rate = calculateNewValue(settings.fadeValues.rate, direction);
          break;
      }
      break;
    case Codes::Preset::Pulse:
      switch(code) {
        case Codes::PulseValue::Rate:
          newValue = settings.pulseValues.rate = calculateNewValue(settings.pulseValues.rate, direction);
          break;
        case Codes::PulseValue::Hue:
          newValue = settings.pulseValues.hue = calculateNewValue(settings.pulseValues.hue, direction);
          break;
        case Codes::PulseValue::Saturation:
          newValue = settings.pulseValues.saturation = calculateNewValue(settings.pulseValues.saturation, direction);
          break;
      }
      break;
  }
  Events::emitValueEvent(settings.preset, code, newValue);
}

void State::controlUp() {
  switch(settings.currentControl) {
    case Codes::Control::Brightness:
      if (settings.brightness < MAX_BRIGHTNESS) {
        settings.brightness += BRIGHTNESS_STEP;
        if (settings.brightness > MAX_BRIGHTNESS) {
          settings.brightness = MAX_BRIGHTNESS;
        }
        Events::emitBrightnessEvent(settings.brightness);
      }
      break;
    case Codes::Control::Preset:
      switch(settings.preset) {
        case Codes::Preset::Fade:
          settings.preset = Codes::Preset::Pulse;
          break;
        case Codes::Preset::Pulse:
          settings.preset = Codes::Preset::Fade;
          break;
      }
      Events::emitPresetEvent(settings.preset);
      break;
    case Codes::Control::Value1:
      handleValueChange(0, true);
      break;
    case Codes::Control::Value2:
      handleValueChange(1, true);
      break;
    case Codes::Control::Value3:
      handleValueChange(2, true);
      break;
  }
}

void State::controlDown() {
  switch(settings.currentControl) {
    case Codes::Control::Brightness:
      if (settings.brightness > 0) {
        settings.brightness -= BRIGHTNESS_STEP;
        if (settings.brightness < 0) {
          settings.brightness = 0;
        }
        Events::emitBrightnessEvent(settings.brightness);
      }
      break;
    case Codes::Control::Preset:
      switch(settings.preset) {
        case Codes::Preset::Fade:
          settings.preset = Codes::Preset::Pulse;
          break;
        case Codes::Preset::Pulse:
          settings.preset = Codes::Preset::Fade;
          break;
      }
      Events::emitPresetEvent(settings.preset);
      break;
    case Codes::Control::Value1:
      handleValueChange(0, false);
      break;
    case Codes::Control::Value2:
      handleValueChange(1, false);
      break;
    case Codes::Control::Value3:
      handleValueChange(2, false);
      break;
  }
}

void State::setClientsConnected(int numClients) {
  if (numClients == settings.numClients) {
    return;
  }
  settings.numClients = numClients;
  Events::emitClientEvent(settings.numClients);
}

void State::setActive() {
  isActive = true;
  settings.idleState = Codes::IdleState::Active;
  Events::emitIdleEvent(settings.idleState);
}

void State::setIdling() {
  idleCountdown = DIM_TIMEOUT;
  isActive = false;
}

void State::init() {
  Serial.println("State initialized");
}

void State::loop() {
  if (!isActive && idleCountdown > 0) {
    idleCountdown--;
    if (!idleCountdown) {
      switch(settings.idleState) {
        case Codes::IdleState::Active:
          idleCountdown = OFF_TIMEOUT;
          settings.idleState = Codes::IdleState::ShallowIdle;
          Events::emitIdleEvent(settings.idleState);
          break;
        case Codes::IdleState::ShallowIdle:
          settings.idleState = Codes::IdleState::DeepIdle;
          Events::emitIdleEvent(settings.idleState);
          break;
      }
    }
  }
}
