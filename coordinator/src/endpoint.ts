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

import { createServer, Socket } from 'net';
import state from './state';
import { MessageType, FadeValue, PulseValue } from './codes';
import { runOperationByPreset, runOperationForEachFadeValue, runOperationForEachPulseValue } from './util';

const connectedClients: Socket[] = [];

export default function init(cb: () => void) {

  const server = createServer((socket) => {
    connectedClients.push(socket);
    socket.on('close', () => {
      const socketIndex = connectedClients.indexOf(socket);
      if (socketIndex === -1) {
        throw new Error('Could not find socket in connected sockets list');
      }
      connectedClients.splice(socketIndex, 1);
      state.clientDisconnected();
    });
    state.clientConnected();
    updateAll();
  }).on('error', (err) => {
    console.error(err);
  });

  server.listen(3000, () => {
    state.on('brightness', updateBrightness);
    state.on('preset', updatePreset);
    state.on('value', updateValue);
    updateAll();
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

function write(buffer: Buffer) {
  for (const connectedClient of connectedClients) {
    connectedClient.write(buffer);
  }
}
