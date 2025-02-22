#!/usr/bin/env node
/*
Copyright (c) Bryan Hughes <bryan@nebri.us>


This file is part of RVL Firmware.

RVL Firmware is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RVL Firmware is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RVL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

import { SerialPort } from 'serialport';
import inquirer from 'inquirer';
import { writeFileSync } from 'fs';

const portsInfo = (await SerialPort.list()).filter((portInfo) => !portInfo.path.startsWith('/dev/ttyS'));

let portPath;
if (portsInfo.length === 0) {
  console.error('Could not find a serial port to listen on!');
} else if (portsInfo.length === 1) {
  portPath = portsInfo[0].path;
} else {
  portPath = (await inquirer.prompt({
    type: 'list',
    name: 'portPath',
    message: 'Which port would you like to monitor?',
    choices: portsInfo.map((port) => port.path)
  })).portPath;
}

const serialport = new SerialPort({
  path: portPath,
  baudRate: 112500
});

serialport.on('error', (err) => {
  console.log('Error: ', err.message);
});

const BUFFER_SIZE = 1000;

const buffer = [];
let timeout;
serialport.on('data', (data) => {
  buffer.push(data.toString());
  while (buffer.length > BUFFER_SIZE) {
    buffer.shift();
  }

  clearTimeout(timeout);
  timeout = setTimeout(() => {
    console.log(`Controller stopped responding at ${new Date().toLocaleString()}`);
    writeFileSync('log.txt', buffer.join(''))
  }, 10000);
});
