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

import { existsSync, readdirSync, readFileSync, writeFileSync, statSync } from 'fs';
import { join, sep } from 'path';
import { execSync } from 'child_process';
import { fileURLToPath } from 'url'
import { dirname } from 'path'

const __dirname = dirname(fileURLToPath(import.meta.url))

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
  -l  --lint      lint the source code
  -b  --build     build the firmware before flashing the target
  -f  --flash     flash the firmware after building the target
  -d  --debug     spin up OpenOCD to allow GDN connections
  -p  --port      serial port to use for flashing/debugging ESP8266 devices
  -c  --console   open a serial port and log debugging information
      --compiledb generate compile_commands.json file
      --help      display this help and exit
`);
}

let lint = false;
let build = false;
let flash = false;
let debug = false;
let log = false;
let compiledb = false;

let target = 'controller';
let port;

let i = 0;
while (i < args.length) {
  switch (args[i]) {
    case '--help':
      showHelp();
      process.exit(0);
    case '-l':
    case '--lint':
      lint = true;
      break;
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
    case '-c':
    case '--console':
      log = true;
      break;
    case '--compiledb':
      compiledb = true;
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

function exec(command, env) {
  try {
    execSync(command, {
      stdio: 'inherit',
      cwd: __dirname,
      env
    });
  } catch(e) {
    process.exit(-1);
  }
}

function findFiles(dir, pattern) {
  const files = readdirSync(dir);
  const foundFiles = [];
  for (const file of files) {
    const filePath = join(dir, file);
    if (statSync(filePath).isDirectory()) {
      foundFiles.push(...findFiles(filePath, pattern));
    } else if (filePath.match(pattern)) {
      foundFiles.push(filePath);
    }
  }
  return foundFiles;
}

function checkHeaderGuard(file) {
  if (!file.endsWith('.hpp') && !file.endsWith('.h')) {
    return;
  }
  const contents = readFileSync(file, 'utf-8').split('\n');
  let ifdef;
  let def;
  const splitFile = file.replace(/-/g, '_').toUpperCase().split(sep);
  const headerGuardSegment = splitFile.slice(splitFile.lastIndexOf('SRC') + 1);
  const filename = headerGuardSegment.pop().replace('.HPP', '').replace('.H', '');
  headerGuardSegment.push(...filename.split('_'), 'H_');
  for (let line of contents) {
    line = line.trimEnd();
    if (ifdef && def) {
      break;
    }
    if (line.startsWith('#ifndef') && !ifdef) {
      ifdef = line.split(' ')[1];
    }
    if (line.startsWith('#define') && !def) {
      def = line.split(' ')[1];
    }
  }
  function guard(segments) {
    return segments.join('_');
  }
  let error = false;
  if (headerGuardSegment.join('_') !== ifdef) {
    error = true;
    console.error(`Invalid #ifdef header guard ${ifdef} in ${file}. Expected ${guard(headerGuardSegment)}`);
  } else if (headerGuardSegment.join('_') !== def) {
    error = true;
    console.error(`Invalid #define header guard ${def} in ${file}. Expected ${guard(headerGuardSegment)}`);
  }
  return error;
}

const SOURCE_FILES = [
  ...findFiles(join(__dirname, 'src'), /(\.cpp|\.hpp|\.c|\.h)$/),
  ...findFiles(join(__dirname, 'lib', 'rvl', 'src'), /(\.cpp|\.hpp|\.c|\.h)$/),
  ...findFiles(join(__dirname, 'lib', 'rvl-wifi', 'src'), /(\.cpp|\.hpp|\.c|\.h)$/),
];

if (compiledb || !existsSync(join(__dirname, 'compile_commands.json'))) {
  console.log('Generating compile_commands.json\n');
  exec('platformio run -e compiledb -t compiledb');
  const commandsPath = join(__dirname, 'compile_commands.json');
  const commands = readFileSync(commandsPath, 'utf-8')
    .replace(/ -I[^\s]*?\.platformio[^\s]*?newlib/g, '');
  writeFileSync(commandsPath, commands);
}

if (lint) {
  console.log(`Linting\n`);
  const guardError = SOURCE_FILES.reduce((error, sourceFile) => {
    return checkHeaderGuard(sourceFile) || error;
  }, false);
  if (guardError) {
    process.exit(-1);
  }
  exec(`clang-tidy ${SOURCE_FILES.join(' ')}`, {
    CPATH: ''
  });
}

if (build) {
  console.log(`Building ${target}\n`);
  exec(`platformio run -e ${target}`);
}

if (flash) {
  console.log(`Flashing ${target}\n`);
  if (!existsSync(targetUrl)) {
    error(`unknown or unbuilt target "${target}".\n`);
  }
  if (target === 'controller') {
    console.log(`\nFlashing target ${target} using JTAG\n`);
    exec(`openocd -f configs/c232hm.cfg -f configs/esp-wroom-32.cfg -c "program_esp ${targetUrl} 0x10000 verify exit"`);
  } else if (target === 'hub' || target === 'receiver') {
    console.log(`\nFlashing target ${target} using UART${port ? ` on ${port}` : ''}\n`);
    exec(`esptool.py${port ? ` -p ${port}` : ''} -b 921600 write_flash 0x0 ${targetUrl}`);
  }
}

if (debug) {
  if (target === 'controller') {
    console.log(`\nCreating debug connection using JTAG\n`);
    exec(`openocd -f configs/c232hm.cfg -f configs/esp-wroom-32.cfg`);
  } else {
    console.error(`\nDebugging is not supported on target ${target}`);
    process.exit(-1);
  }
}

if (log) {
  console.log(`\nOpening serial port for debugging${port ? ` on ${port}` : ''}\n`);
  exec(`seriallog${port ? ` -p ${port}` : ''}`);
}
