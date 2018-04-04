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
const dgram_1 = require("dgram");
const async_1 = require("async");
const state_1 = require("./state");
const codes_1 = require("./codes");
const util_1 = require("./util");
const PORT = 3000;
const CLIENT_TIMEOUT = 5000;
const CLIENT_HEARTBEAT_CODE = 126;
const connectedClients = {};
const writeBuffer = [];
const server = dgram_1.createSocket('udp4');
server.on('message', (message, rinfo) => {
    if (message.length !== 1 || message[0] !== CLIENT_HEARTBEAT_CODE) {
        return;
    }
    if (connectedClients[rinfo.address]) {
        clearTimeout(connectedClients[rinfo.address]);
    }
    else {
        console.log(`Client ${rinfo.address} connected`);
        state_1.default.clientConnected();
        setTimeout(() => updateAll(), 100);
    }
    connectedClients[rinfo.address] = setTimeout(() => {
        console.log(`Client ${rinfo.address} disconnected`);
        delete connectedClients[rinfo.address];
        state_1.default.clientDisconnected();
    }, CLIENT_TIMEOUT);
});
function init(cb) {
    server.bind({ port: PORT }, () => {
        state_1.default.on('brightness', updateBrightness);
        state_1.default.on('preset', updatePreset);
        state_1.default.on('value', updateValue);
        updateAll();
        console.log('Endpoint server listening');
        cb();
    });
}
exports.default = init;
function updateAll() {
    const settings = state_1.default.getSettings();
    updateBrightness(settings.brightness);
    updatePreset(settings.preset);
}
function updateAllValues() {
    const settings = state_1.default.getSettings();
    util_1.runOperationByPreset(settings.preset, {
        Fade: () => util_1.runOperationForEachFadeValue({
            Rate: () => updateValue({ code: codes_1.FadeValue.Rate, value: settings.fadeValues.rate })
        }),
        Pulse: () => util_1.runOperationForEachPulseValue({
            Rate: () => updateValue({ code: codes_1.PulseValue.Rate, value: settings.pulseValues.rate }),
            Hue: () => updateValue({ code: codes_1.PulseValue.Hue, value: settings.pulseValues.hue }),
            Saturation: () => updateValue({ code: codes_1.PulseValue.Saturation, value: settings.pulseValues.saturation })
        })
    });
}
function updateBrightness(brightness) {
    console.log(`Setting brightness to ${brightness}`);
    write(Buffer.from([codes_1.MessageType.SetBrightness, brightness]));
}
function updatePreset(preset) {
    console.log(`Setting preset to ${preset}`);
    write(Buffer.from([codes_1.MessageType.SetPreset, preset]));
    updateAllValues();
}
function updateValue({ code, value }) {
    console.log(`Setting value ${code} to ${value}`);
    write(Buffer.from([codes_1.MessageType.SetValue, code, value]));
}
let isWriting = false;
function write(buffer) {
    const connectedClientAddresses = Object.keys(connectedClients);
    if (connectedClientAddresses.length === 0) {
        isWriting = false;
        return;
    }
    if (isWriting) {
        writeBuffer.push(buffer);
        return;
    }
    isWriting = true;
    async_1.parallel(connectedClientAddresses.map((address) => (next) => {
        server.send(buffer, PORT, address, next);
    }), () => {
        isWriting = false;
        const nextBuffer = writeBuffer.shift();
        if (nextBuffer) {
            write(nextBuffer);
        }
    });
}
//# sourceMappingURL=endpoint.js.map