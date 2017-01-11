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
var net_1 = require("net");
var state_1 = require("./state");
var codes_1 = require("./codes");
var util_1 = require("./util");
var connectedClients = [];
function init(cb) {
    var server = net_1.createServer(function (socket) {
        connectedClients.push(socket);
        socket.on('close', function () {
            var socketIndex = connectedClients.indexOf(socket);
            if (socketIndex === -1) {
                throw new Error('Could not find socket in connected sockets list');
            }
            connectedClients.splice(socketIndex, 1);
            state_1.default.clientDisconnected();
        });
        state_1.default.clientConnected();
        updateAll();
    }).on('error', function (err) {
        console.error(err);
    });
    server.listen(3000, function () {
        state_1.default.on('brightness', updateBrightness);
        state_1.default.on('preset', updatePreset);
        state_1.default.on('value', updateValue);
        updateAll();
        cb();
    });
}
Object.defineProperty(exports, "__esModule", { value: true });
exports.default = init;
function updateAll() {
    var settings = state_1.default.getSettings();
    updateBrightness(settings.brightness);
    updatePreset(settings.preset);
}
function updateAllValues() {
    var settings = state_1.default.getSettings();
    util_1.runOperationByPreset(settings.preset, {
        Fade: function () { return util_1.runOperationForEachFadeValue({
            Rate: function () { return updateValue({ code: codes_1.FadeValue.Rate, value: settings.fadeValues.rate }); }
        }); },
        Pulse: function () { return util_1.runOperationForEachPulseValue({
            Rate: function () { return updateValue({ code: codes_1.PulseValue.Rate, value: settings.pulseValues.rate }); },
            Hue: function () { return updateValue({ code: codes_1.PulseValue.Hue, value: settings.pulseValues.hue }); },
            Saturation: function () { return updateValue({ code: codes_1.PulseValue.Saturation, value: settings.pulseValues.saturation }); }
        }); }
    });
}
function updateBrightness(brightness) {
    console.log("Setting brightness to " + brightness);
    write(Buffer.from([codes_1.MessageType.SetBrightness, brightness]));
}
function updatePreset(preset) {
    console.log("Setting preset to " + preset);
    write(Buffer.from([codes_1.MessageType.SetPreset, preset]));
    updateAllValues();
}
function updateValue(_a) {
    var code = _a.code, value = _a.value;
    console.log("Setting value " + code + " to " + value);
    write(Buffer.from([codes_1.MessageType.SetValue, code, value]));
}
function write(buffer) {
    for (var _i = 0, connectedClients_1 = connectedClients; _i < connectedClients_1.length; _i++) {
        var connectedClient = connectedClients_1[_i];
        connectedClient.write(buffer);
    }
}
