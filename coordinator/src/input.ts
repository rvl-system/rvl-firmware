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

import { Control, IdleState } from './codes';
import state from './state';
import { Button } from 'johnny-five';

const HOLD_ENGAGE_TIME = 1500;
const HOLD_RATE = 200;
const STEP = 4;

const NEXT_BUTTON = 'GPIO4';
const UP_BUTTON = 'GPIO17';
const DOWN_BUTTON = 'GPIO27';

export default function init(cb: () => void) {

  const nextControlButton = new Button(NEXT_BUTTON);
  nextControlButton.on('down', () => {
    if (state.getSettings().idleState === IdleState.Active) {
      state.nextControl();
    }
    state.setActive();
  });
  nextControlButton.on('up', () => state.setIdling());

  const controlUpButton = new Button(UP_BUTTON);
  let controlUpTimeout: NodeJS.Timer;
  let controlUpInterval: NodeJS.Timer;
  controlUpButton.on('down', () => {
    if (state.getSettings().idleState === IdleState.Active) {
      state.controlUp(1);
      if (state.getSettings().currentControl !== Control.Preset) {
        controlUpTimeout = setTimeout(() => {
          controlUpInterval = setInterval(() => state.controlUp(STEP), HOLD_RATE);
        }, HOLD_ENGAGE_TIME);
      }
    }
    state.setActive();
  });
  controlUpButton.on('up', () => {
    state.setIdling();
    clearTimeout(controlUpTimeout);
    clearInterval(controlUpInterval);
  });

  const controlDownButton = new Button(DOWN_BUTTON);
  let controlDownTimeout: NodeJS.Timer;
  let controlDownInterval: NodeJS.Timer;
  controlDownButton.on('down', () => {
    if (state.getSettings().idleState === IdleState.Active) {
      state.controlDown(1);
      if (state.getSettings().currentControl !== Control.Preset) {
        controlDownTimeout = setTimeout(() => {
          controlDownInterval = setInterval(() => state.controlDown(STEP), HOLD_RATE);
        }, HOLD_ENGAGE_TIME);
      }
    }
    state.setActive();
  });
  controlDownButton.on('up', () => {
    state.setIdling();
    clearTimeout(controlDownTimeout);
    clearInterval(controlDownInterval);
  });

  setImmediate(cb);
}
