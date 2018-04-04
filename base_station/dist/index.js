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
const async_1 = require("async");
const johnny_five_1 = require("johnny-five");
// tslint:disable-next-line:no-require-imports
const Raspi = require("raspi-io");
const screen_1 = require("./screen");
const input_1 = require("./input");
const endpoint_1 = require("./endpoint");
const state_1 = require("./state");
const board = new johnny_five_1.Board({
    io: new Raspi({
        excludePins: [
            'MOSI0',
            'MISO0',
            'SCLK0',
            'CE0'
        ]
    })
});
board.on('ready', () => {
    async_1.parallel([
        (next) => screen_1.default(board, next),
        (next) => input_1.default(next),
        (next) => endpoint_1.default(next)
    ], () => {
        state_1.default.setActive();
        state_1.default.setIdling();
        console.log('Running');
    });
});
//# sourceMappingURL=index.js.map