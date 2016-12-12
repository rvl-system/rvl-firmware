'use strict';

const async = require('async');
const five = require('johnny-five');
const raspi = require('raspi-io');

const screen = require('./screen');
const input = require('./input');
const endpoint = require('./endpoint');

const board = new five.Board({
  io: new raspi({
    excludePins: [
      'MOSI0',
      'MISO0',
      'SCLK0',
      'CE0'
    ]
  })
});
board.on('ready', () => {
  async.parallel([
    (next) => screen.init(board, five, next),
    (next) => input.init(board, next),
    (next) => endpoint.init(next)
    // eslint-disable-next-line no-console
  ], () => console.log('Running'));
});
