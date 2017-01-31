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

import { createSocket } from 'dgram';
import { parallel } from 'async';
import state from './state';
import { MessageType, FadeValue, PulseValue } from './codes';
import { runOperationByPreset, runOperationForEachFadeValue, runOperationForEachPulseValue } from './util';

const PORT = 3000;
const CLIENT_TIMEOUT = 5000;
const CLIENT_HEARTBEAT_CODE = 126;

const connectedClients: { [ address: string ]: NodeJS.Timer } = {};
const writeBuffer: Buffer[] = [];

const server = createSocket('udp4');

server.on('message', (message: Buffer, rinfo) => {
  if (message.length !== 1 || message[0] !== CLIENT_HEARTBEAT_CODE) {
    return;
  }
  if (connectedClients[rinfo.address]) {
    clearTimeout(connectedClients[rinfo.address]);
  } else {
    console.log(`Client ${rinfo.address} connected`);
    state.clientConnected();
    setTimeout(() => updateAll(), 100);
  }

  connectedClients[rinfo.address] = setTimeout(() => {
    console.log(`Client ${rinfo.address} disconnected`);
    delete connectedClients[rinfo.address];
    state.clientDisconnected();
  }, CLIENT_TIMEOUT);
});

export default function init(cb: () => void) {
  server.bind({ port: PORT }, () => {
    state.on('brightness', updateBrightness);
    state.on('preset', updatePreset);
    state.on('value', updateValue);
    updateAll();
    console.log('Endpoint server listening');
    cb();
  });
}

function updateAll() {
  const settings = state.getSettings();
  updateBrightness(settings.brightness);
  updatePreset(settings.preset);
}

function updateAllValues() {
  const settings = state.getSettings();
  runOperationByPreset(settings.preset, {
    Fade: () => runOperationForEachFadeValue({
      Rate: () => updateValue({ code: FadeValue.Rate, value: settings.fadeValues.rate })
    }),
    Pulse: () => runOperationForEachPulseValue({
      Rate: () => updateValue({ code: PulseValue.Rate, value: settings.pulseValues.rate }),
      Hue: () => updateValue({ code: PulseValue.Hue, value: settings.pulseValues.hue }),
      Saturation: () => updateValue({ code: PulseValue.Saturation, value: settings.pulseValues.saturation })
    })
  });
}

function updateBrightness(brightness: number) {
  console.log(`Setting brightness to ${brightness}`);
  write(Buffer.from([ MessageType.SetBrightness, brightness ]));
}

function updatePreset(preset: number) {
  console.log(`Setting preset to ${preset}`);
  write(Buffer.from([ MessageType.SetPreset, preset ]));
  updateAllValues();
}

function updateValue({ code, value }: { code: number, value: number }) {
  console.log(`Setting value ${code} to ${value}`);
  write(Buffer.from([ MessageType.SetValue, code, value ]));
}

let isWriting = false;
function write(buffer: Buffer) {
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
  parallel(connectedClientAddresses.map((address) => (next: () => void) => {
    server.send(buffer, PORT, address, next);
  }), () => {
    isWriting = false;
    const nextBuffer = writeBuffer.shift();
    if (nextBuffer) {
      write(nextBuffer);
    }
  });
}
