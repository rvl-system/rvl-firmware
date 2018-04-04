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
Object.defineProperty(exports, "__esModule", { value: true });
const codes_1 = require("./codes");
const state_1 = require("./state");
const johnny_five_1 = require("johnny-five");
const HOLD_ENGAGE_TIME = 1500;
const HOLD_RATE = 200;
const STEP = 4;
const NEXT_BUTTON = 'GPIO17';
const UP_BUTTON = 'GPIO27';
const DOWN_BUTTON = 'GPIO22';
function init(cb) {
    const nextControlButton = new johnny_five_1.Button(NEXT_BUTTON);
    nextControlButton.on('down', () => {
        if (state_1.default.getSettings().idleState === codes_1.IdleState.Active) {
            state_1.default.nextControl();
        }
        state_1.default.setActive();
    });
    nextControlButton.on('up', () => state_1.default.setIdling());
    const controlUpButton = new johnny_five_1.Button(UP_BUTTON);
    let controlUpTimeout;
    let controlUpInterval;
    controlUpButton.on('down', () => {
        if (state_1.default.getSettings().idleState === codes_1.IdleState.Active) {
            state_1.default.controlUp(1);
            if (state_1.default.getSettings().currentControl !== codes_1.Control.Preset) {
                controlUpTimeout = setTimeout(() => {
                    controlUpInterval = setInterval(() => state_1.default.controlUp(STEP), HOLD_RATE);
                }, HOLD_ENGAGE_TIME);
            }
        }
        state_1.default.setActive();
    });
    controlUpButton.on('up', () => {
        state_1.default.setIdling();
        clearTimeout(controlUpTimeout);
        clearInterval(controlUpInterval);
    });
    const controlDownButton = new johnny_five_1.Button(DOWN_BUTTON);
    let controlDownTimeout;
    let controlDownInterval;
    controlDownButton.on('down', () => {
        if (state_1.default.getSettings().idleState === codes_1.IdleState.Active) {
            state_1.default.controlDown(1);
            if (state_1.default.getSettings().currentControl !== codes_1.Control.Preset) {
                controlDownTimeout = setTimeout(() => {
                    controlDownInterval = setInterval(() => state_1.default.controlDown(STEP), HOLD_RATE);
                }, HOLD_ENGAGE_TIME);
            }
        }
        state_1.default.setActive();
    });
    controlDownButton.on('up', () => {
        state_1.default.setIdling();
        clearTimeout(controlDownTimeout);
        clearInterval(controlDownInterval);
    });
    setImmediate(cb);
}
exports.default = init;
//# sourceMappingURL=input.js.map