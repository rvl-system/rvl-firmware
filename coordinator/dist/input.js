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
var codes_1 = require("./codes");
var state_1 = require("./state");
var johnny_five_1 = require("johnny-five");
var HOLD_ENGAGE_TIME = 1500;
var HOLD_RATE = 200;
var STEP = 4;
var NEXT_BUTTON = 'GPIO17';
var UP_BUTTON = 'GPIO27';
var DOWN_BUTTON = 'GPIO22';
function init(cb) {
    var nextControlButton = new johnny_five_1.Button(NEXT_BUTTON);
    nextControlButton.on('down', function () {
        if (state_1.default.getSettings().idleState === codes_1.IdleState.Active) {
            state_1.default.nextControl();
        }
        state_1.default.setActive();
    });
    nextControlButton.on('up', function () { return state_1.default.setIdling(); });
    var controlUpButton = new johnny_five_1.Button(UP_BUTTON);
    var controlUpTimeout;
    var controlUpInterval;
    controlUpButton.on('down', function () {
        if (state_1.default.getSettings().idleState === codes_1.IdleState.Active) {
            state_1.default.controlUp(1);
            if (state_1.default.getSettings().currentControl !== codes_1.Control.Preset) {
                controlUpTimeout = setTimeout(function () {
                    controlUpInterval = setInterval(function () { return state_1.default.controlUp(STEP); }, HOLD_RATE);
                }, HOLD_ENGAGE_TIME);
            }
        }
        state_1.default.setActive();
    });
    controlUpButton.on('up', function () {
        state_1.default.setIdling();
        clearTimeout(controlUpTimeout);
        clearInterval(controlUpInterval);
    });
    var controlDownButton = new johnny_five_1.Button(DOWN_BUTTON);
    var controlDownTimeout;
    var controlDownInterval;
    controlDownButton.on('down', function () {
        if (state_1.default.getSettings().idleState === codes_1.IdleState.Active) {
            state_1.default.controlDown(1);
            if (state_1.default.getSettings().currentControl !== codes_1.Control.Preset) {
                controlDownTimeout = setTimeout(function () {
                    controlDownInterval = setInterval(function () { return state_1.default.controlDown(STEP); }, HOLD_RATE);
                }, HOLD_ENGAGE_TIME);
            }
        }
        state_1.default.setActive();
    });
    controlDownButton.on('up', function () {
        state_1.default.setIdling();
        clearTimeout(controlDownTimeout);
        clearInterval(controlDownInterval);
    });
    setImmediate(cb);
}
Object.defineProperty(exports, "__esModule", { value: true });
exports.default = init;
