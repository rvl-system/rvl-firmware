/*
Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>

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

'use strict';

const Oled = require('oled-js');
const font = require('oled-font-5x7');

const state = require('./state');
const codes = require('./codes');

const MAX_BRIGHTNESS = 255;
const MAX_VALUE = 255;

const LCD_WIDTH = 128;
const LCD_HEIGHT = 64;
const TEXT_HEIGHT = 7;

const PADDING = 4;
const BAR_SEPARATION = 5;
const PRESET_SEPARATION = 10;
const HIGHIGHT_PADDING = 3;

const BRIGHTNESS_ICON_SIZE = 11;
const BRIGHTNESS_ICON_X = PADDING;
const BRIGHTNESS_ICON_Y = LCD_HEIGHT - PADDING - BRIGHTNESS_ICON_SIZE;

const BRIGHTNESS_BAR_X = PADDING;
const BRIGHTNESS_BAR_Y = PADDING;
const BRIGHTNESS_BAR_WIDTH = 11;
const BRIGHTNESS_BAR_HEIGHT = BRIGHTNESS_ICON_Y - BAR_SEPARATION - PADDING;

const BRIGHTNESS_HIGHLIGHT_X = BRIGHTNESS_BAR_X - HIGHIGHT_PADDING;
const BRIGHTNESS_HIGHLIGHT_Y = BRIGHTNESS_BAR_Y - HIGHIGHT_PADDING;
const BRIGHTNESS_HIGHLIGHT_WIDTH = BRIGHTNESS_BAR_WIDTH + HIGHIGHT_PADDING * 2;
const BRIGHTNESS_HIGHLIGHT_HEIGHT = BRIGHTNESS_BAR_HEIGHT + BRIGHTNESS_ICON_SIZE + BAR_SEPARATION + HIGHIGHT_PADDING * 2;

const PRESET_LABEL_X = BRIGHTNESS_ICON_X + BRIGHTNESS_BAR_WIDTH + PRESET_SEPARATION;
const PRESET_LABEL_Y = PADDING;
const PRESET_VALUE_X = PRESET_LABEL_X + 53;
const PRESET_VALUE_Y = PADDING;

const PRESET_HIGHLIGHT_X = PRESET_LABEL_X - HIGHIGHT_PADDING;
const PRESET_HIGHLIGHT_Y = PRESET_LABEL_Y - HIGHIGHT_PADDING;
const PRESET_HIGHLIGHT_WIDTH = LCD_WIDTH - PRESET_HIGHLIGHT_X - PADDING + HIGHIGHT_PADDING - 1;
const PRESET_HIGHLIGHT_HEIGHT = TEXT_HEIGHT + HIGHIGHT_PADDING * 2;

const VALUE_RIGHT_X = LCD_WIDTH - PADDING - 15;
const VALUE_LABEL_X = BRIGHTNESS_ICON_X + BRIGHTNESS_BAR_WIDTH + PRESET_SEPARATION;
const VALUE_WIDTH = VALUE_RIGHT_X - VALUE_LABEL_X;
const VALUE_BAR_X = VALUE_LABEL_X + 17;
const VALUE_BAR_WIDTH = VALUE_RIGHT_X - VALUE_BAR_X;
const VALUE_HIGHLIGHT_X = VALUE_LABEL_X - HIGHIGHT_PADDING;
const VALUE_HIGHLIGHT_WIDTH = VALUE_WIDTH + HIGHIGHT_PADDING * 2;
const VALUE_HIGHLIGHT_HEIGHT = TEXT_HEIGHT + HIGHIGHT_PADDING * 2;

const VALUE1_Y = PRESET_VALUE_Y + PRESET_HIGHLIGHT_HEIGHT + 3;
const VALUE1_HIGHLIGHT_Y = VALUE1_Y - HIGHIGHT_PADDING;

const VALUE2_Y = VALUE1_Y + PRESET_HIGHLIGHT_HEIGHT + 3;
const VALUE2_HIGHLIGHT_Y = VALUE2_Y - HIGHIGHT_PADDING;

const VALUE3_Y = VALUE2_Y + PRESET_HIGHLIGHT_HEIGHT + 3;
const VALUE3_HIGHLIGHT_Y = VALUE3_Y - HIGHIGHT_PADDING;

const COUNT_X = LCD_WIDTH - PADDING - TEXT_HEIGHT;
const COUNT_Y = VALUE2_Y + 6;

let oled;
let previousControl;

module.exports = {
  init
};

function init(board, five, cb) {
  oled = new Oled(board, five, {
    width: 128,
    height: 64,
    address: 0x3C
  });

  // Clear the screen
  oled.fillRect(0, 0, 127, 63, 0, false);

  // Draw the brightness icon
  drawBrightnessIcon(BRIGHTNESS_ICON_X, BRIGHTNESS_ICON_Y, BRIGHTNESS_ICON_SIZE);

  // Draw the preset label:
  oled.setCursor(PRESET_LABEL_X, PRESET_LABEL_Y);
  oled.writeString(font, 1, 'Preset: ', 1, true, 2);

  // Update the screen
  oled.update();

  const currentSettings = state.getSettings();
  updateControl(currentSettings.currentControl);
  updateBrightness(currentSettings.brightness);
  updatePreset(currentSettings.preset);
  updateClientCount(currentSettings.numClients);

  state.on('control', updateControl);
  state.on('brightness', updateBrightness);
  state.on('preset', updatePreset);
  state.on('value', updateValue);
  state.on('client', updateClientCount);

  setImmediate(cb);
}

function drawControlOutline(control, color) {
  switch (control) {
    case codes.control.BRIGHTNESS:
      oled.drawRect(BRIGHTNESS_HIGHLIGHT_X, BRIGHTNESS_HIGHLIGHT_Y, BRIGHTNESS_HIGHLIGHT_WIDTH - 1, BRIGHTNESS_HIGHLIGHT_HEIGHT, color, false);
      break;
    case codes.control.PRESET:
      oled.drawRect(PRESET_HIGHLIGHT_X, PRESET_HIGHLIGHT_Y, PRESET_HIGHLIGHT_WIDTH - 1, PRESET_HIGHLIGHT_HEIGHT, color, false);
      break;
    case codes.control.VALUE1:
      oled.drawRect(VALUE_HIGHLIGHT_X, VALUE1_HIGHLIGHT_Y, VALUE_HIGHLIGHT_WIDTH - 1, VALUE_HIGHLIGHT_HEIGHT, color, false);
      break;
    case codes.control.VALUE2:
      oled.drawRect(VALUE_HIGHLIGHT_X, VALUE2_HIGHLIGHT_Y, VALUE_HIGHLIGHT_WIDTH - 1, VALUE_HIGHLIGHT_HEIGHT, color, false);
      break;
    case codes.control.VALUE3:
      oled.drawRect(VALUE_HIGHLIGHT_X, VALUE3_HIGHLIGHT_Y, VALUE_HIGHLIGHT_WIDTH - 1, VALUE_HIGHLIGHT_HEIGHT, color, false);
      break;
  }
}

function drawVerticalBar(x, y, w, h, percent) {
  const fillHeight = Math.round((h - 2) * percent);
  const emptyHeight = h - 2 - fillHeight;

  // Draw the border. drawRect w is x + w, but fillRect is x + w - 1, so normalize here
  oled.drawRect(x, y, w - 1, h, 1, false);

  // Draw the empty section
  if (emptyHeight !== 0) {
    oled.fillRect(
      x + 1,
      y + 1,
      w - 2,
      emptyHeight,
      0, false);
  }

  // Draw the filled section
  if (fillHeight !== 0) {
    oled.fillRect(
      x + 1,
      y + 1 + emptyHeight,
      w - 2,
      fillHeight,
      1, false);
  }
}

function drawHorizontalBar(x, y, w, h, percent) {
  const fillWidth = Math.round((w - 2) * percent);
  const emptyWidth = w - 2 - fillWidth;

  // Draw the border. drawRect w is x + w, but fillRect is x + w - 1, so normalize here
  oled.drawRect(x, y, w - 1, h, 1, false);

  // Draw the filled section
  if (fillWidth !== 0) {
    oled.fillRect(
      x + 1,
      y + 1,
      fillWidth,
      h - 2,
      1, false);
  }

  // Draw the empty section
  if (emptyWidth !== 0) {
    oled.fillRect(
      x + 1 + fillWidth,
      y + 1,
      emptyWidth,
      h - 2,
      0, false);
  }
}

function drawBrightnessIcon(x, y, size) {
  const midX = x + Math.floor(size / 2);
  const midY = y + Math.floor(size / 2);
  const indent = Math.round(size / 8);
  let radius = Math.round(size / 4);

  oled.drawLine(midX, y, midX, y + size - 1, 1, false);
  oled.drawLine(x, midY, x + size - 1, midY, 1, false);
  oled.drawLine(x + indent, y + indent, x + size - 1 - indent, y + size - 1 - indent, 1, false);
  oled.drawLine(x + indent, y + size - 1 - indent, x + size - 1 - indent, y + 1, 1, false);

  while(radius > 1) {
    oled.drawCircle(midX, midY, radius, 1, false);
    radius--;
  }
}

function drawString(x, y, w, string) {
  oled.fillRect(x, y, w, TEXT_HEIGHT, 0, false);
  oled.setCursor(x, y);
  oled.writeString(font, 1, string, 1, true, 2, false);
}

function updateControl(control) {
  drawControlOutline(previousControl, 0);
  drawControlOutline(control, 1);
  previousControl = control;
  oled.update();
}

function updateBrightness(brightness) {
  drawVerticalBar(BRIGHTNESS_BAR_X, BRIGHTNESS_BAR_Y, BRIGHTNESS_BAR_WIDTH, BRIGHTNESS_BAR_HEIGHT, brightness / MAX_BRIGHTNESS);
  oled.update();
}

function updatePreset(preset) {
  drawString(PRESET_VALUE_X, PRESET_VALUE_Y, 127 - PRESET_VALUE_X - PADDING,
    codes.getCodeName('preset', preset));

  const presets = Object.keys(codes.value[preset]);
  const settings = state.getSettings();
  if (presets[0]) {
    drawString(VALUE_LABEL_X, VALUE1_Y, VALUE_WIDTH, presets[0][0] + ':');
    drawHorizontalBar(VALUE_BAR_X, VALUE1_Y, VALUE_BAR_WIDTH, TEXT_HEIGHT, settings.values[preset][presets[0]] / MAX_VALUE);
  } else {
    oled.fillRect(VALUE_LABEL_X, VALUE1_Y, VALUE_WIDTH, TEXT_HEIGHT, 0, false);
  }
  if (presets[1]) {
    drawString(VALUE_LABEL_X, VALUE2_Y, VALUE_WIDTH, presets[1][0] + ':');
    drawHorizontalBar(VALUE_BAR_X, VALUE2_Y, VALUE_BAR_WIDTH, TEXT_HEIGHT, settings.values[preset][presets[1]] / MAX_VALUE);
  } else {
    oled.fillRect(VALUE_LABEL_X, VALUE2_Y, VALUE_WIDTH, TEXT_HEIGHT, 0, false);
  }
  if (presets[2]) {
    drawString(VALUE_LABEL_X, VALUE3_Y, VALUE_WIDTH, presets[2][0] + ':');
    drawHorizontalBar(VALUE_BAR_X, VALUE3_Y, VALUE_BAR_WIDTH, TEXT_HEIGHT, settings.values[preset][presets[2]] / MAX_VALUE);
  } else {
    oled.fillRect(VALUE_LABEL_X, VALUE3_Y, VALUE_WIDTH, TEXT_HEIGHT, 0, false);
  }

  oled.update();
}

function updateValue({ code, value }) {
  switch(code) {
    case 0:
      drawHorizontalBar(VALUE_BAR_X, VALUE1_Y, VALUE_BAR_WIDTH, TEXT_HEIGHT, value / MAX_VALUE);
      break;
    case 1:
      drawHorizontalBar(VALUE_BAR_X, VALUE2_Y, VALUE_BAR_WIDTH, TEXT_HEIGHT, value / MAX_VALUE);
      break;
    case 2:
      drawHorizontalBar(VALUE_BAR_X, VALUE3_Y, VALUE_BAR_WIDTH, TEXT_HEIGHT, value / MAX_VALUE);
      break;
  }

  oled.update();
}

function updateClientCount(count) {
  drawString(COUNT_X, COUNT_Y, TEXT_HEIGHT, count.toString());
  oled.update();
}
