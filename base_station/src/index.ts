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

import { parallel } from 'async';
import { Board } from 'johnny-five';
// tslint:disable-next-line:no-require-imports
import Raspi = require('raspi-io');

import screenInit from './screen';
import inputInit from './input';
import endpointInit from './endpoint';
import state from './state';

const board = new Board({
  io: new Raspi({
    excludePins: [
      'MOSI0',
      'MISO0',
      'SCLK0',
      'CE0'
    ]
  })
} as any);

board.on('ready', () => {
  parallel([
    (next) => screenInit(board, next),
    (next) => inputInit(next),
    (next) => endpointInit(next)
  ], () => {
    state.setActive();
    state.setIdling();
    console.log('Running');
  });
});
