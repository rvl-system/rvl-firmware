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
#include "SSD1306Brzo.h"
#include "config.h"
#include "screen.h"
#include "state.h"
#include "codes.h"

#define PIXEL_ON WHITE
#define PIXEL_OFF BLACK

#define ACTIVE_CONTRAST 255
#define SHALLOW_IDLE_CONTRAST 16

#define MAX_BRIGHTNESS 255
#define MAX_VALUE 255

#define TEXT_HEIGHT 7

#define PADDING 4
#define BAR_SEPARATION 5
#define PRESET_SEPARATION 10
#define HIGHIGHT_PADDING 3

#define BRIGHTNESS_ICON_SIZE 11

const int BRIGHTNESS_ICON_X = PADDING;
const int BRIGHTNESS_ICON_Y = LCD_HEIGHT - PADDING - BRIGHTNESS_ICON_SIZE;

const int BRIGHTNESS_BAR_X = PADDING;
const int BRIGHTNESS_BAR_Y = PADDING;
const int BRIGHTNESS_BAR_WIDTH = 11;
const int BRIGHTNESS_BAR_HEIGHT = BRIGHTNESS_ICON_Y - BAR_SEPARATION - PADDING;

const int BRIGHTNESS_HIGHLIGHT_X = BRIGHTNESS_BAR_X - HIGHIGHT_PADDING;
const int BRIGHTNESS_HIGHLIGHT_Y = BRIGHTNESS_BAR_Y - HIGHIGHT_PADDING;
const int BRIGHTNESS_HIGHLIGHT_WIDTH = BRIGHTNESS_BAR_WIDTH + HIGHIGHT_PADDING * 2;
const int BRIGHTNESS_HIGHLIGHT_HEIGHT = BRIGHTNESS_BAR_HEIGHT + BRIGHTNESS_ICON_SIZE + BAR_SEPARATION + HIGHIGHT_PADDING * 2;

const int PRESET_LABEL_X = BRIGHTNESS_ICON_X + BRIGHTNESS_BAR_WIDTH + PRESET_SEPARATION;
const int PRESET_LABEL_Y = PADDING;
const int PRESET_LABEL_WIDTH = 37;
const int PRESET_VALUE_X = PRESET_LABEL_X + PRESET_LABEL_WIDTH;
const int PRESET_VALUE_Y = PADDING;

const int PRESET_HIGHLIGHT_X = PRESET_LABEL_X - HIGHIGHT_PADDING;
const int PRESET_HIGHLIGHT_Y = PRESET_LABEL_Y - HIGHIGHT_PADDING;
const int PRESET_HIGHLIGHT_WIDTH = LCD_WIDTH - PRESET_HIGHLIGHT_X - PADDING + HIGHIGHT_PADDING - 1;
const int PRESET_HIGHLIGHT_HEIGHT = TEXT_HEIGHT + HIGHIGHT_PADDING * 2;

const int VALUE_RIGHT_X = LCD_WIDTH - PADDING - 15;
const int VALUE_LABEL_X = BRIGHTNESS_ICON_X + BRIGHTNESS_BAR_WIDTH + PRESET_SEPARATION;
const int VALUE_WIDTH = VALUE_RIGHT_X - VALUE_LABEL_X;
const int VALUE_BAR_X = VALUE_LABEL_X + 17;
const int VALUE_BAR_WIDTH = VALUE_RIGHT_X - VALUE_BAR_X;
const int VALUE_HIGHLIGHT_X = VALUE_LABEL_X - HIGHIGHT_PADDING;
const int VALUE_HIGHLIGHT_WIDTH = VALUE_WIDTH + HIGHIGHT_PADDING * 2;
const int VALUE_HIGHLIGHT_HEIGHT = TEXT_HEIGHT + HIGHIGHT_PADDING * 2;

const int VALUE1_Y = PRESET_VALUE_Y + PRESET_HIGHLIGHT_HEIGHT + 3;
const int VALUE1_HIGHLIGHT_Y = VALUE1_Y - HIGHIGHT_PADDING;

const int VALUE2_Y = VALUE1_Y + PRESET_HIGHLIGHT_HEIGHT + 3;
const int VALUE2_HIGHLIGHT_Y = VALUE2_Y - HIGHIGHT_PADDING;

const int VALUE3_Y = VALUE2_Y + PRESET_HIGHLIGHT_HEIGHT + 3;
const int VALUE3_HIGHLIGHT_Y = VALUE3_Y - HIGHIGHT_PADDING;

const int COUNT_X = LCD_WIDTH - PADDING - TEXT_HEIGHT;
const int COUNT_Y = VALUE2_Y + 6;

Codes::Control::Control previousControl = Codes::Control::None;
SSD1306Brzo display(LCD_ADDRESS, LCD_SDA, LCD_SCL);

void drawControlOutline(Codes::Control::Control control, OLEDDISPLAY_COLOR color) {
  display.setColor(color);
  switch(control) {
    case Codes::Control::Brightness:
      display.drawRect(BRIGHTNESS_HIGHLIGHT_X, BRIGHTNESS_HIGHLIGHT_Y, BRIGHTNESS_HIGHLIGHT_WIDTH - 1, BRIGHTNESS_HIGHLIGHT_HEIGHT);
      break;
    case Codes::Control::Preset:
      display.drawRect(PRESET_HIGHLIGHT_X, PRESET_HIGHLIGHT_Y, PRESET_HIGHLIGHT_WIDTH - 1, PRESET_HIGHLIGHT_HEIGHT);
      break;
    case Codes::Control::Value1:
      display.drawRect(VALUE_HIGHLIGHT_X, VALUE1_HIGHLIGHT_Y, VALUE_HIGHLIGHT_WIDTH - 1, VALUE_HIGHLIGHT_HEIGHT);
      break;
    case Codes::Control::Value2:
      display.drawRect(VALUE_HIGHLIGHT_X, VALUE2_HIGHLIGHT_Y, VALUE_HIGHLIGHT_WIDTH - 1, VALUE_HIGHLIGHT_HEIGHT);
      break;
    case Codes::Control::Value3:
      display.drawRect(VALUE_HIGHLIGHT_X, VALUE3_HIGHLIGHT_Y, VALUE_HIGHLIGHT_WIDTH - 1, VALUE_HIGHLIGHT_HEIGHT);
      break;
  }
}

void drawVerticalBar(int x, int y, int w, int h, double percent) {
  const int fillHeight = (h - 2) * percent;
  const int emptyHeight = h - 2 - fillHeight;

  // Draw the border. drawRect w is x + w, but fillRect is x + w - 1, so normalize here
  display.drawRect(x, y, w - 1, h);

  // Draw the empty section
  display.setColor(PIXEL_OFF);
  if (emptyHeight != 0) {
    display.fillRect(
      x + 1,
      y + 1,
      w - 3,
      emptyHeight);
  }

  // Draw the filled section
  display.setColor(PIXEL_ON);
  if (fillHeight != 0) {
    display.fillRect(
      x + 1,
      y + 1 + emptyHeight,
      w - 3,
      fillHeight);
  }
}

void drawHorizontalBar(int x, int y, int w, int h, double percent) {
  int fillWidth = (w - 3) * percent;
  int emptyWidth = w - 3 - fillWidth;

  display.setColor(PIXEL_ON);
  display.drawRect(x, y, w - 1, h);

  // Draw the empty section
  display.setColor(PIXEL_OFF);
  if (emptyWidth != 0) {
    display.fillRect(
      x + 1 + fillWidth,
      y + 1,
      emptyWidth,
      h - 2);
  }

  // Draw the filled section
  display.setColor(PIXEL_ON);
  if (fillWidth != 0) {
    display.fillRect(
      x + 1,
      y + 1,
      fillWidth,
      h - 2);
  }
}

void drawBrightnessIcon(int x, int y, int size) {
  const int midX = x + size / 2;
  const int midY = y + size / 2;
  const int indent = size / 8;
  int radius = size / 3;

  display.setColor(PIXEL_ON);
  display.drawLine(midX, y, midX, y + size - 1);
  display.drawLine(x, midY, x + size - 1, midY);
  display.drawLine(x + indent, y + indent, x + size - 1 - indent, y + size - 1 - indent);
  display.drawLine(x + indent, y + size - 1 - indent, x + size - 1 - indent, y + 1);
  display.fillCircle(midX + 1, midY, radius);
}

void drawString(int x, int y, int w, const char* text) {
  display.setColor(PIXEL_OFF);
  display.fillRect(x, y, w, TEXT_HEIGHT);
  display.setColor(PIXEL_ON);
  display.drawString(x, y - 3, text);
}

void Screen::init() {
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_10);

  // Draw the brightness icon
  drawBrightnessIcon(BRIGHTNESS_ICON_X, BRIGHTNESS_ICON_Y, BRIGHTNESS_ICON_SIZE);

  // Draw the preset label:
  drawString(PRESET_LABEL_X, PRESET_LABEL_Y, PRESET_LABEL_WIDTH, "Preset: ");

  auto currentSettings = State::getSettings();
  updateControl(currentSettings->currentControl);
  updateBrightness(currentSettings->brightness);
  updatePreset(currentSettings->preset);
  updateClientCount(currentSettings->numClients);

  display.display();

  Serial.println("Screen initialized");
}

void Screen::loop() {
}

void Screen::updateControl(Codes::Control::Control control) {
  drawControlOutline(previousControl, PIXEL_OFF);
  drawControlOutline(control, PIXEL_ON);
  previousControl = control;
  display.display();
}

void Screen::updateBrightness(int brightness) {
  drawVerticalBar(BRIGHTNESS_BAR_X, BRIGHTNESS_BAR_Y, BRIGHTNESS_BAR_WIDTH, BRIGHTNESS_BAR_HEIGHT, (double)brightness / (double)MAX_BRIGHTNESS);
  display.display();
}

void Screen::updatePreset(Codes::Preset::Preset preset) {
  State::Settings* settings = State::getSettings();
  switch(preset) {
    case Codes::Preset::Fade:
      display.setColor(PIXEL_ON);
      drawString(PRESET_VALUE_X, PRESET_VALUE_Y, 127 - PRESET_VALUE_X - PADDING, "Fade");
      drawString(VALUE_LABEL_X, VALUE1_Y, VALUE_WIDTH, "R:");
      updateValue(Codes::Preset::Fade, Codes::FadeValue::Rate, settings->fadeValues.rate);
      display.setColor(PIXEL_OFF);
      display.fillRect(VALUE_LABEL_X, VALUE2_Y, VALUE_WIDTH, TEXT_HEIGHT);
      display.fillRect(VALUE_LABEL_X, VALUE3_Y, VALUE_WIDTH, TEXT_HEIGHT);
      break;
    case Codes::Preset::Pulse:
      display.setColor(PIXEL_ON);
      drawString(PRESET_VALUE_X, PRESET_VALUE_Y, 127 - PRESET_VALUE_X - PADDING, "Pulse");
      drawString(VALUE_LABEL_X, VALUE1_Y, VALUE_WIDTH, "R:");
      updateValue(Codes::Preset::Pulse, Codes::PulseValue::Rate, settings->pulseValues.rate);
      drawString(VALUE_LABEL_X, VALUE2_Y, VALUE_WIDTH, "H:");
      updateValue(Codes::Preset::Pulse, Codes::PulseValue::Hue, settings->pulseValues.hue);
      drawString(VALUE_LABEL_X, VALUE3_Y, VALUE_WIDTH, "S:");
      updateValue(Codes::Preset::Pulse, Codes::PulseValue::Saturation, settings->pulseValues.saturation);
      break;
  }

  display.display();
}

void Screen::updateValue(Codes::Preset::Preset preset, int code, int value) {
  switch (code) {
    case 0:
      drawHorizontalBar(VALUE_BAR_X, VALUE1_Y, VALUE_BAR_WIDTH, TEXT_HEIGHT, (double)value / (double)MAX_VALUE);
      break;
    case 1:
      drawHorizontalBar(VALUE_BAR_X, VALUE2_Y, VALUE_BAR_WIDTH, TEXT_HEIGHT, (double)value / (double)MAX_VALUE);
      break;
    case 2:
      drawHorizontalBar(VALUE_BAR_X, VALUE3_Y, VALUE_BAR_WIDTH, TEXT_HEIGHT, (double)value / (double)MAX_VALUE);
      break;
  }

  display.display();
}

void Screen::updateClientCount(int count) {
  char convertedCount[3];
  itoa(count, convertedCount, 10);
  if (count < 10) {
    convertedCount[1] = 0;
  } else {
    convertedCount[2] = 0;
  }
  drawString(COUNT_X, COUNT_Y, TEXT_HEIGHT, convertedCount);
  display.display();
}

void Screen::updateIdleState(Codes::IdleState::IdleState idleState) {
  switch(idleState) {
    case Codes::IdleState::Active:
      display.setContrast(ACTIVE_CONTRAST);
      display.displayOn();
      break;
    case Codes::IdleState::ShallowIdle:
      display.setContrast(SHALLOW_IDLE_CONTRAST);
      display.displayOn();
      break;
    case Codes::IdleState::DeepIdle:
      display.displayOff();
      break;
  }
}
