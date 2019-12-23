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
  -b  --no-build  do not build the firmware before flashing the target
  -f  --no-flash  do not flash the firmware after building the target
  -j  --jtag      flash the firmware using a JTAG adapter
  -p  --port      the TTY port to use, can be supplied multiple times to flash
                  multiple boards simultaneously. If this parameter is not
                  supplied, this script will attempt to guess the port.
                  Cannot be used with --jtag
      --help      display this help and exit
`);
}

let i = 0;
let useJTAG = false;
let ports = [];
let build = true;
let flash = true;
let target;

while (i < args.length) {
  switch (args[i]) {
    case '--help':
      showHelp();
      process.exit(0);
    case '-b':
    case '--no-build':
      build = false;
      break;
    case '-f':
    case '--no-flash':
      flash = false;
      break;
    case '-j':
    case '--jtag':
      useJTAG = true;
      break;
    case '-p':
    case '--port': {
      const port = args[++i];
      if (!existsSync(port)) {
        error(`unknown port ${port}`);
      }
      ports.push(port);
      break;
    }
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
// TODO: this will fail on first build always
const targetUrl = join(__dirname, '.pio', 'build', target, 'firmware.bin');
if (!existsSync(targetUrl)) {
  error(`unknown or unbuilt target "${target}".\n`);
}

// If neither useJTAG was supplied and a port wasn't supplied, let's try and
// autodetect the port
if (!useJTAG && !ports.length) {
  for (const prefix of [ '/dev/ttyUSB', '/dev/ttyACM' ]) {
    for (let port = 0; port < 10; port++) {
      if (existsSync(prefix + port)) {
        ports.push(prefix + port);
        break;
      }
    }
  }
}

// Make sure we don't have conflicting arguments
if (useJTAG && ports.length) {
  error('--jtag and --port cannot be used together');
}

// Make sure we have a way to flash, if we are flashing
if (flash && (!useJTAG && ports.length === 0)) {
  error('no port supplied or JTAG use specified');
}

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
  if (useJTAG) {
    console.log(`\nFlashing target ${target} using JTAG\n`);
    exec(`openocd -f scripts/c232hm.cfg -f scripts/esp-wroom-32.cfg -c "program_esp .pio/build/${target}/firmware.bin 0x10000 verify exit"`);
  } else {
    if (ports.length === 1) {
      console.log(`Flashing target ${target} using serial on port ${ports[0]}`);
    } else {
      console.log(`Flashing target ${target} using serial on ports ${ports}`);
    }
    for (const port of ports) {
      exec(`esptool.py --chip esp32 -p ${port} -b 921600 write_flash 0x10000 .pio/build/${target}/firmware.bin`);
    }
  }
}
