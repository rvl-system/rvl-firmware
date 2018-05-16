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
#include <brzo_i2c.h>
#include <SSD1306Brzo.h>
#include "./ui/screen.h"
#include "../config.h"
#include "./state.h"
#include "./codes.h"
#include "./event.h"

#include "./ui/controls/brightness.h"
#include "./ui/controls/preset.h"
#include "./ui/controls/value.h"

#define BRIGHTNESS_STEP 1
#define MAX_BRIGHTNESS 255

#define BRIGHTNESS_X 0
#define BRIGHTNESS_Y 0
#define BRIGHTNESS_WIDTH 15
#define BRIGHTNESS_HEIGHT 64

#define PRESET_X 17
#define PRESET_Y 0
#define PRESET_HEIGHT 13
#define PRESET_WIDTH 111

#define VALUE_X 17
#define VALUE_SPACING 13
#define VALUE_HEIGHT 13
#define VALUE_WIDTH 96

#define COUNT_X 122
#define COUNT_Y 40

namespace Screen {

uint8 currentControl = 0;

SSD1306Brzo display(LCD_ADDRESS, LCD_SDA, LCD_SCL);

void update();

void init() {
  Event::on(Codes::EventType::AnimationChange, update);
  display.init();
#ifdef INVERT_DISPLAY
  display.flipScreenVertically();
#endif
  display.clear();
  display.setFont(ArialMT_Plain_10);

  Screen::update();

  Serial.println("Screen initialized");
}

void loop() {
}

void nextControl() {
  int maxControls = 3;
  for (int i = 0; i < NUM_PRESET_VALUES; i++) {
    if (presetValueLabels[State::getSettings()->presetSettings.preset][i] == NULL) {
      break;
    }
    maxControls++;
  }
  currentControl++;
  if (currentControl == maxControls) {
    currentControl = 0;
  }

  Serial.print("Setting control ");
  Serial.println(currentControl);
  update();
}

int calculateNewValue(byte code, int value, bool direction) {
  auto settings = State::getSettings();
  if (direction) {
    value++;
    if (value > presetValueMax[settings->presetSettings.preset][code]) {
      value = presetValueMax[settings->presetSettings.preset][code];
    }
  } else {
    value--;
    if (value < presetValueMin[settings->presetSettings.preset][code]) {
      value = presetValueMin[settings->presetSettings.preset][code];
    }
  }
  return value;
}

void handleValueChange(int code, bool direction) {
  auto settings = State::getSettings();
  int newValue = calculateNewValue(
    code,
    settings->presetSettings.presetValues[settings->presetSettings.preset][code],
    direction);
  settings->presetSettings.presetValues[settings->presetSettings.preset][code] = newValue;

  Serial.print("Setting preset ");
  Serial.print(settings->presetSettings.preset);
  Serial.print(" code ");
  Serial.print(code);
  Serial.print(" to value ");
  Serial.println(newValue);
  Event::emit(Codes::EventType::AnimationChange);
}

void controlUp() {
  auto settings = State::getSettings();
  switch (currentControl) {
    case Codes::Control::Brightness:
      if (settings->brightness < MAX_BRIGHTNESS) {
        settings->brightness += BRIGHTNESS_STEP;
        if (settings->brightness > MAX_BRIGHTNESS) {
          settings->brightness = MAX_BRIGHTNESS;
        }
        Serial.print("Setting brightness to ");
        Serial.println(settings->brightness);
        Event::emit(Codes::EventType::AnimationChange);
      }
      break;
    case Codes::Control::Preset:
      settings->presetSettings.preset++;
      if (settings->presetSettings.preset == NUM_PRESETS) {
        settings->presetSettings.preset = 0;
      }
      Serial.print("Setting preset ");
      Serial.println(settings->presetSettings.preset);
      Event::emit(Codes::EventType::AnimationChange);
      break;
    default:
      handleValueChange(currentControl - 3, true);
      break;
  }
  update();
}

void controlDown() {
  auto settings = State::getSettings();
  switch (currentControl) {
    case Codes::Control::Brightness:
      if (settings->brightness > 0) {
        settings->brightness -= BRIGHTNESS_STEP;
        if (settings->brightness < 0) {
          settings->brightness = 0;
        }
        Serial.print("Setting brightness to ");
        Serial.println(settings->brightness);
        Event::emit(Codes::EventType::AnimationChange);
      }
      break;
    case Codes::Control::Preset:
      switch (settings->presetSettings.preset) {
        case Codes::Preset::Fade:
          settings->presetSettings.preset = Codes::Preset::Pulse;
          break;
        case Codes::Preset::Pulse:
          settings->presetSettings.preset = Codes::Preset::Fade;
          break;
      }
      Serial.print("Setting preset ");
      Serial.println(settings->presetSettings.preset);
      Event::emit(Codes::EventType::AnimationChange);
      break;
    default:
      handleValueChange(currentControl - 3, false);
      break;
  }
  update();
}

void update() {
  auto settings = State::getSettings();
  display.clear();
  display.setColor(WHITE);

  // Draw the brightness icon
  BrightnessControl::render(
    display,
    BRIGHTNESS_X,
    BRIGHTNESS_Y,
    BRIGHTNESS_WIDTH,
    BRIGHTNESS_HEIGHT,
    currentControl == Codes::Control::Brightness,
    settings->brightness);

  // Draw the preset
  PresetControl::render(
    display,
    PRESET_X,
    PRESET_Y,
    PRESET_WIDTH,
    PRESET_HEIGHT,
    currentControl == Codes::Control::Preset,
    presetNames[settings->presetSettings.preset]);

  // Draw the values
  for (int i = 0; i < NUM_PRESET_VALUES; i++) {
    const char* label = presetValueLabels[settings->presetSettings.preset][i];
    if (label != NULL) {
      ValueControl::render(
        display,
        VALUE_X,
        (i + 1) * VALUE_SPACING,
        VALUE_WIDTH,
        VALUE_HEIGHT,
        currentControl == i + 3,
        label,
        static_cast<double>(
          settings->presetSettings.presetValues[settings->presetSettings.preset][i] -
          presetValueMin[settings->presetSettings.preset][i]) / static_cast<double>(
          presetValueMax[settings->presetSettings.preset][i] -
          presetValueMin[settings->presetSettings.preset][i]));
    }
  }

  display.display();
}

}  // namespace Screen
