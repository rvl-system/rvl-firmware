'use strict';

const EventEmitter = require('events');
const codes = require('./codes');

const BRIGHTNESS_STEP = 7;
const MAX_BRIGHTNESS = 255;
const VALUE_STEP = 4;
const MAX_VALUE = 255;

const settings = {
  preset: 0,
  brightness: 10,
  values: {},
  currentControl: 0,
  numClients: 0
};

for (const preset in codes.preset) {
  const presetCode = codes.preset[preset];
  settings.values[presetCode] = {};
  for (const value in codes.value[presetCode]) {
    settings.values[presetCode][value] = codes.defaultValue[presetCode][value];
  }
}

const state = new EventEmitter();

state.nextControl = () => {
  settings.currentControl++;
  const numControls = 1 +
    Object.keys(codes.preset).length +
    Object.keys(codes.value[settings.preset]).length;
  if (settings.currentControl === numControls) {
    settings.currentControl = 0;
  }
  state.emit('control', settings.currentControl);
};

state.controlUp = (steps) => {

  function handleValue(code) {
      const value = Object.keys(codes.value[settings.preset])[code];
      const presetValues = settings.values[settings.preset];
      if (presetValues[value] >= MAX_VALUE) {
        return;
      }
      // TODO: this is mock steps
      presetValues[value] += VALUE_STEP * steps;
      if (presetValues[value] > MAX_VALUE) {
        presetValues[value] = MAX_VALUE;
      }
      state.emit('value', {
        code,
        value: presetValues[value]
      });
  }

  switch(settings.currentControl) {
    case codes.control.BRIGHTNESS:
      if (settings.brightness < MAX_BRIGHTNESS) {
        settings.brightness += BRIGHTNESS_STEP * steps;
        if (settings.brightness > MAX_BRIGHTNESS) {
          settings.brightness = MAX_BRIGHTNESS;
        }
        state.emit('brightness', settings.brightness);
      }
      break;
    case codes.control.PRESET:
      settings.preset++;
      if (settings.preset === Object.keys(codes.preset).length) {
        settings.preset = 0;
      }
      state.emit('preset', settings.preset);
      break;
    case codes.control.VALUE1:
      handleValue(0);
      break;
    case codes.control.VALUE2:
      handleValue(1);
      break;
    case codes.control.VALUE3:
      handleValue(2);
      break;
  }
};

state.controlDown = (steps) => {

  function handleValue(code) {
      const value = Object.keys(codes.value[settings.preset])[code];
      const presetValues = settings.values[settings.preset];
      if (presetValues[value] <= 0) {
        return;
      }
      // TODO: this is mock steps
      presetValues[value] -= VALUE_STEP * steps;
      if (presetValues[value] < 0) {
        presetValues[value] = 0;
      }
      state.emit('value', {
        code,
        value: presetValues[value]
      });
  }

  switch(settings.currentControl) {
    case codes.control.BRIGHTNESS:
      if (settings.brightness > 0) {
        settings.brightness -= BRIGHTNESS_STEP * steps;
        if (settings.brightness < 0) {
          settings.brightness = 0;
        }
        state.emit('brightness', settings.brightness);
      }
      break;
    case codes.control.PRESET:
      settings.preset--;
      if (settings.preset < 0) {
        settings.preset = Object.keys(codes.preset).length - 1;
      }
      state.emit('preset', settings.preset);
      break;
    case codes.control.VALUE1:
      handleValue(0);
      break;
    case codes.control.VALUE2:
      handleValue(1);
      break;
    case codes.control.VALUE3:
      handleValue(2);
      break;
  }
};

state.clientConnected = () => {
  settings.numClients++;
};

state.clientDisconnected = () => {
  settings.numClients--;
};

state.getSettings = () => Object.assign({}, settings);

module.exports = state;
