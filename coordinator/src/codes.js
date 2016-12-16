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

const FADE = 0;
const PULSE = 1;

module.exports = {
  messageTypes: {
    SET_PRESET: 0,
    SET_VALUE: 1,
    SET_BRIGHTNESS: 2
  },
  preset: {
    FADE,
    PULSE
  },
  control: {
    NONE: 0,
    BRIGHTNESS: 1,
    PRESET: 2,
    VALUE1: 3,
    VALUE2: 4,
    VALUE3: 5
  },
  value: {
    [FADE]: {
      RATE: 0
    },
    [PULSE]: {
      RATE: 0,
      HUE: 1,
      SATURATION: 2
    }
  },
  defaultValue: {
    [FADE]: {
      RATE: 20
    },
    [PULSE]: {
      RATE: 20,
      HUE: 0,
      SATURATION: 100
    }
  },
  getCodeName(type, code) {
    const keys = Object.keys(module.exports[type]);
    for (const key of keys) {
      if (module.exports[type][key] === code) {
        return key;
      }
    }
  }
};
