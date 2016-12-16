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

const net = require('net');
const rgb2hsv = require('color2color').rgb2hsv;
const express = require('express');
const bodyParser = require('body-parser');

const app = express();

const CLIENT_IPS = [
  // '192.168.1.129',
  '192.168.1.116'
];

const sockets = CLIENT_IPS.map(() => new net.Socket());

app.use(express.static(`${__dirname}/static`));
app.use(bodyParser.json());

let preset = 'pulse';
const values = {
  fade: {
    rate: 64
  },
  pulse: {
    rate: 64,
    color: '#942192'
  }
};

let messageQueue = [];
let isSending = false;
function sendMessage(newMessage) {
  messageQueue.push(newMessage);
  if (!isSending) {
    pump();
  }
  function pump() {
    const sendBuffer = Buffer.concat(messageQueue);
    messageQueue = [];
    isSending = true;
    let sem = sockets.length;
    sockets.forEach((socket) => {
      socket.write(sendBuffer, () => {
        if (!(--sem)) {
          isSending = false;
          if (messageQueue.length) {
            pump();
          }
        }
      });
    });
  }
}

function writePreset() {
  console.log('Changing preset');
  switch (preset) {
    case 'fade':
      sendMessage(Buffer.from([0, 0]));
      break;
    case 'pulse':
      sendMessage(Buffer.from([0, 1]));
      break;
  }
}

function writeFadeSpeed() {
  console.log('Changing fade speed');
  sendMessage(Buffer.from([1, 0, values.fade.rate]));
}

function writePulseSpeed() {
  console.log('Changing pulse speed');
  sendMessage(Buffer.from([1, 0, values.pulse.rate]));
}

function writePulseColor() {
  console.log('Changing pulse color');
  const color = values.pulse.color;
  const r = parseInt(color.substring(1,3), 16);
  const g = parseInt(color.substring(3,5), 16);
  const b = parseInt(color.substring(5,7), 16);
  const hsv = rgb2hsv(r, g, b);
  const hue = Math.round(hsv[0] / 360 * 255);
  const saturation = Math.round(hsv[1] * 255);
  sendMessage(Buffer.from([1, 1, hue]));
  sendMessage(Buffer.from([1, 2, saturation]));
}

app.get('/api/preset', (req, res) => {
  res.send(preset);
});

app.post('/api/preset', (req, res) => {
  preset = req.body.value;
  writePreset();
  res.send('ok');
});

app.get('/api/values', (req, res) => {
  res.set('Content-Type', 'application/json');
  res.send(values);
});

app.post('/api/fade_speed', (req, res) => {
  values.fade.rate = parseInt(req.body.value, 10);
  writeFadeSpeed();
  res.send('ok');
});

app.post('/api/pulse_speed', (req, res) => {
  values.pulse.rate = parseInt(req.body.value, 10);
  writePulseSpeed();
  res.send('ok');
});

app.post('/api/pulse_color', (req, res) => {
  values.pulse.color = req.body.value;
  writePulseColor();
  res.send('ok');
});


let semaphore = sockets.length;
for (let i = 0; i < sockets.length; i++) {
  sockets[i].connect({
    host: CLIENT_IPS[i],
    port: 80
  }, () => {
    if (!(--semaphore)) {
      app.listen(3000, () => {
        console.log('Light controller listening on port 3000!');
      });
    }
  });
}
