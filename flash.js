#!/usr/bin/env node
/*
Copyright (c) 2017 Bryan Hughes <bryan@nebri.us>

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

const { existsSync } = require('fs');
const { join } = require('path');
const { execSync } = require('child_process');

const args = process.argv.slice(2);

function error(message) {
  console.error(`Error: ${message}\n`);
  showHelp();
  process.exit(-1);
}

function showHelp() {
  console.log(
`Usage: ./flash.js [OPTIONS].. [TARGET]

Builds and flashses the firemware supplied by TARGET to an RVL board. TARGET is
the name of the requested target as specified in an [env:TARGET] section of
platformio.ini.

OPTIONS:
  -b  --build  build the firmware before flashing the target
  -f  --flash  flash the firmware after building the target
  -d  --debug  spin up OpenOCD to allow GDN connections
  -p  --port   serial port to use for flashing/debugging ESP8266 devices
      --help      display this help and exit
`);
}

let build = false;
let flash = false;
let debug = false;
let target = 'controller';
let port;

let i = 0;
while (i < args.length) {
  switch (args[i]) {
    case '--help':
      showHelp();
      process.exit(0);
    case '-b':
    case '--build':
      build = true;
      break;
    case '-f':
    case '--flash':
      flash = true;
      break;
    case '-d':
    case '--debug':
      debug = true;
      break;
    case '-p':
    case '--port':
      port = args[++i];
      break;
    default:
      target = args[i];
      break;
  }
  i++;
}

// Validate the target
if (typeof target !== 'string') {
  error(`no value for TARGET was supplied.\n`);
}
const targetUrl = `.pio/build/${target}/firmware.bin`;

function exec(command) {
  try {
    execSync(command, {
      stdio: 'inherit',
      cwd: __dirname
    });
  } catch(e) {
    process.exit(-1);
  }
}

if (build) {
  console.log(`\nBuilding target ${target}\n`);
  for (const dir of [ 'lib/rvl/src', 'lib/rvl-wifi/src', 'src' ]) {
    exec(`cpplint --quiet --linelength=120 --recursive --repository=${join(__dirname, dir)} ${dir}/*`);
  }
  exec(`platformio run -e ${target}`);
}

if (flash) {
  if (!existsSync(targetUrl)) {
    error(`unknown or unbuilt target "${target}".\n`);
  }
  if (target === 'controller') {
    console.log(`\nFlashing target ${target} using JTAG\n`);
    exec(`openocd -f scripts/c232hm.cfg -f scripts/esp-wroom-32.cfg -c "program_esp ${targetUrl} 0x10000 verify exit"`);
  } else if (target === 'hub' || target === 'receiver') {
    console.log(`\nFlashing target ${target} using UART${port ? ` on ${port}` : ''}\n`);
    exec(`esptool.py${port ? ` -p ${port}` : ''} -b 921600 write_flash 0x0 ${targetUrl}`);
  }
}

if (debug) {
  if (target === 'controller') {
    console.log(`\nCreating debug connection using JTAG\n`);
    exec(`openocd -f scripts/c232hm.cfg -f scripts/esp-wroom-32.cfg`);
  } else {
    console.log(`\nOpening serial port for debugging${port ? ` on ${port}` : ''}\n`);
    exec(`seriallog${port ? ` -p ${port}` : ''}`);
  }
}
