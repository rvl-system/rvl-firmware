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
var dgram_1 = require("dgram");
var async_1 = require("async");
var state_1 = require("./state");
var codes_1 = require("./codes");
var util_1 = require("./util");
var PORT = 3000;
var CLIENT_TIMEOUT = 5000;
var CLIENT_HEARTBEAT_CODE = 126;
var connectedClients = {};
var writeBuffer = [];
var server = dgram_1.createSocket('udp4');
server.on('message', function (message, rinfo) {
    if (message.length !== 1 || message[0] !== CLIENT_HEARTBEAT_CODE) {
        return;
    }
    if (connectedClients[rinfo.address]) {
        clearTimeout(connectedClients[rinfo.address]);
    }
    else {
        console.log("Client " + rinfo.address + " connected");
        state_1.default.clientConnected();
        setTimeout(function () { return updateAll(); }, 100);
    }
    connectedClients[rinfo.address] = setTimeout(function () {
        console.log("Client " + rinfo.address + " disconnected");
        delete connectedClients[rinfo.address];
        state_1.default.clientDisconnected();
    }, CLIENT_TIMEOUT);
});
function init(cb) {
    server.bind({ port: PORT }, function () {
        state_1.default.on('brightness', updateBrightness);
        state_1.default.on('preset', updatePreset);
        state_1.default.on('value', updateValue);
        updateAll();
        console.log('Endpoint server listening');
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
var isWriting = false;
function write(buffer) {
    var connectedClientAddresses = Object.keys(connectedClients);
    if (connectedClientAddresses.length === 0) {
        isWriting = false;
        return;
    }
    if (isWriting) {
        writeBuffer.push(buffer);
        return;
    }
    isWriting = true;
    async_1.parallel(connectedClientAddresses.map(function (address) { return function (next) {
        server.send(buffer, PORT, address, next);
    }; }), function () {
        isWriting = false;
        var nextBuffer = writeBuffer.shift();
        if (nextBuffer) {
            write(nextBuffer);
        }
    });
}
