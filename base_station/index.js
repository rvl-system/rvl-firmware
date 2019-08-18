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

const { RVL } = require('rvl-node');

const RAVER_LIGHTS_INTERFACE = process.argv[2];
const RAVER_LIGHTS_CHANNEL = 0;

if (!RAVER_LIGHTS_INTERFACE) {
  throw new Error('A network interface must be passed as the one and only argument');
}

const rvl = new RVL({
  networkInterface: RAVER_LIGHTS_INTERFACE,
  port: 4978,
  mode: 'receiver',
  logLevel: 'debug',
  channel: RAVER_LIGHTS_CHANNEL,
  enableClockSync: true
});

rvl.on('initialized', () => rvl.start());
