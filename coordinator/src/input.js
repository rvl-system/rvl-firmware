'use strict';

const state = require('./state');
const codes = require('./codes');
const five = require('johnny-five');

module.exports = {
  init
};

const HOLD_ENGAGE_TIME = 1500;
const HOLD_RATE = 200;
const STEP = 4;

const NEXT_BUTTON = 'GPIO13';
const UP_BUTTON = 'GPIO5';
const DOWN_BUTTON = 'GPIO6';

function init(board, cb) {

  const nextControlButton = new five.Button(NEXT_BUTTON);
  nextControlButton.on('press', () => state.nextControl());

  const controlUpButton = new five.Button(UP_BUTTON);
  let controlUpTimeout;
  let controlUpInterval;
  controlUpButton.on('down', () => {
    state.controlUp(1);
    if (state.getSettings().currentControl !== codes.control.PRESET) {
      controlUpTimeout = setTimeout(() => {
        controlUpInterval = setInterval(() => state.controlUp(STEP), HOLD_RATE);
      }, HOLD_ENGAGE_TIME);
    }
  });
  controlUpButton.on('up', () => {
    clearTimeout(controlUpTimeout);
    clearInterval(controlUpInterval);
  });

  const controlDownButton = new five.Button(DOWN_BUTTON);
  let controlDownTimeout;
  let controlDownInterval;
  controlDownButton.on('down', () => {
    state.controlDown(1);
    if (state.getSettings().currentControl !== codes.control.PRESET) {
      controlDownTimeout = setTimeout(() => {
        controlDownInterval = setInterval(() => state.controlDown(STEP), HOLD_RATE);
      }, HOLD_ENGAGE_TIME);
    }
  });
  controlDownButton.on('up', () => {
    clearTimeout(controlDownTimeout);
    clearInterval(controlDownInterval);
  });

  setImmediate(cb);
}
