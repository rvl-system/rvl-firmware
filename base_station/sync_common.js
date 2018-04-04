#!/usr/bin/env node
/*
Copyright (c) 2016-2018 Bryan Hughes <bryan@nebri.us>

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

const fs = require('fs');
const path = require('path');

const DEFINE_REGEX = /^\#define (.*?) (.*?)$/;

const defines = fs.readFileSync(path.join(__dirname, '..', 'common', 'common_config.h'))
  .toString()
  .split('\n')
  .filter((line) => DEFINE_REGEX.test(line))
  .map((line) => DEFINE_REGEX.exec(line));

let rawCodes = fs.readFileSync(path.join(__dirname, '..', 'common', 'codes.h'))
  .toString()
  .replace(/\n/g, ' ')
  .replace(/\s+/g, ' ');
const enums = [];
while(true) {
  if (rawCodes.indexOf('enum') === -1) {
    break;
  }
  rawCodes = rawCodes.substr(rawCodes.indexOf('enum'));
  const match = /(enum [a-zA-Z0-9_]* \{ .*? \})/.exec(rawCodes);
  rawCodes = rawCodes.substr(rawCodes.indexOf('}'));
  enums.push(match[1]);
}

let rawRuntimeDefines = fs.readFileSync(path.join(__dirname, '..', 'common', 'common_config.cpp'))
  .toString()
  .split('\n');

let ipAddresses = rawRuntimeDefines
  .filter((line) => line.startsWith('IPAddress'))
  .map((line) => /IPAddress ([A-Z_]*)\(([0-9, ]*)\)/.exec(line));

let line = 0;
let valueState = 'scanning';
let currentConstruct;
const constructs = [];
while (line < rawRuntimeDefines.length) {
  switch (valueState) {
    case 'scanning':
      const match = /^const [a-z\*]* ([a-zA-Z0-9_]*)\[.*?\]\[.*?\] = \{/.exec(rawRuntimeDefines[line]);
      if (match) {
        currentConstruct = `export const ${match[1]} = [\n`;
        valueState = 'parsing';
      }
      break;
    case 'parsing':
      if (rawRuntimeDefines[line] === '};') {
        currentConstruct += '];\n';
        constructs.push(currentConstruct);
        valueState = 'scanning';
      } else {
        currentConstruct += rawRuntimeDefines[line].replace(/\{/g, '[').replace(/\}/g, ']').replace(/NULL/g, 'null').replace(/"/g, '\'') + '\n'
      }
      break;
  }
  line++;
}

const source =
`/*
Copyright (c) 2016-2018 Bryan Hughes <bryan@nebri.us>

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

${defines.map((line) => `export const ${line[1]} = ${line[2]};`).join('\n')}

${ipAddresses.map((lineMatch) => `export const ${lineMatch[1]} = '${lineMatch[2].replace(/, /g, '.')}';`).join('\n')}

${enums.map((line) => `export ${line}`).join('\n')}

${constructs.join('\n')}`.replace(/"/g, '\'');

fs.writeFileSync(path.join(__dirname, 'src', 'codes.ts'), source);
