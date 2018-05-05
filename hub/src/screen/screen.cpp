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
#include "./screen/screen.h"
#include "./config.h"
#include "./state.h"
#include "./codes.h"
#include "./event.h"

#include "./screen/controls/brightness.h"
#include "./screen/controls/preset.h"
#include "./screen/controls/value.h"

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

SSD1306Brzo display(LCD_ADDRESS, LCD_SDA, LCD_SCL);

class ScreenStateListener : public Event::EventListenerInterface {
 public:
  void onEvent() {
    update();
  }
};

void init() {
  auto listener = new ScreenStateListener();
  Event::on(Codes::EventTypes::AnimationChange, listener);
  Event::on(Codes::EventTypes::InputChange, listener);
  display.init();
  display.clear();
  display.setFont(ArialMT_Plain_10);

  Screen::update();

  Serial.println("Screen initialized");
}

void loop() {
}

void update() {
  State::Settings* settings = State::getSettings();
  display.clear();
  display.setColor(WHITE);

  // // Draw the brightness icon
  BrightnessControl::render(
    display,
    BRIGHTNESS_X,
    BRIGHTNESS_Y,
    BRIGHTNESS_WIDTH,
    BRIGHTNESS_HEIGHT,
    settings->currentControl == Codes::Control::Brightness,
    settings->brightness);

  // Draw the preset
  PresetControl::render(
    display,
    PRESET_X,
    PRESET_Y,
    PRESET_WIDTH,
    PRESET_HEIGHT,
    settings->currentControl == Codes::Control::Preset,
    presetNames[settings->preset]);

  // Draw the values
  for (int i = 0; i < NUM_PRESET_VALUES; i++) {
    const char* label = presetValueLabels[settings->preset][i];
    if (label != NULL) {
      ValueControl::render(
        display,
        VALUE_X,
        (i + 1) * VALUE_SPACING,
        VALUE_WIDTH,
        VALUE_HEIGHT,
        settings->currentControl == i + 3,
        label,
        static_cast<double>(settings->presetValues[settings->preset][i] - presetValueMin[settings->preset][i]) /
          static_cast<double>(presetValueMax[settings->preset][i] - presetValueMin[settings->preset][i]));
    }
  }

  // Draw the number of connected clients
  char convertedCount[2];
  itoa(settings->numClients, convertedCount, 10);
  convertedCount[1] = 0;
  display.drawString(COUNT_X, COUNT_Y, convertedCount);

  display.display();
}

}  // namespace Screen
