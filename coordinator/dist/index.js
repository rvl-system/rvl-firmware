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
var async_1 = require("async");
var johnny_five_1 = require("johnny-five");
// tslint:disable-next-line:no-require-imports
var Raspi = require("raspi-io");
var screen_1 = require("./screen");
var input_1 = require("./input");
var endpoint_1 = require("./endpoint");
var state_1 = require("./state");
var board = new johnny_five_1.Board({
    io: new Raspi({
        excludePins: [
            'MOSI0',
            'MISO0',
            'SCLK0',
            'CE0'
        ]
    })
});
board.on('ready', function () {
    async_1.parallel([
        function (next) { return screen_1.default(board, next); },
        function (next) { return input_1.default(next); },
        function (next) { return endpoint_1.default(next); }
    ], function () {
        state_1.default.setActive();
        state_1.default.setIdling();
        console.log('Running');
    });
});
