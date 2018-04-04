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

export const WIFI_SSID = 'RaverLights';
export const WIFI_PASSPHRASE = '4&7EWf7p3h222;$+wg9]&4dN,dGh4^@}';
export const SERVER_PORT = 3000;
export const UPDATE_RATE = 33;
export const NUM_PRESETS = 3;
export const NUM_PRESET_VALUES = 10;
export const DEFAULT_PRESET = 2;
export const DEFAULT_BRIGHTNESS = 25;

export const SERVER_IP = '192.168.42.1';
export const GATEWAY = '192.168.42.255';
export const SUBNET = '255.255.255.0';

export enum Preset { Fade = 0, Pulse = 1, Wave = 2, Unknown = 255 }
export enum Control { None = 0, Brightness = 1, Preset = 2, Value1 = 3, Value2 = 4, Value3 = 5 }
export enum IdleState { Active = 0, ShallowIdle = 1, DeepIdle = 2 }

export const presetValueLabels = [
  [ 'RATE', null, null, null, null, null, null, null, null, null ],
  [ 'RATE', 'HUE', 'SATR', null, null, null, null, null, null, null ],
  [ 'RATE', 'SPAC', 'HUEF', 'HUEB', null, null, null, null, null, null ]
];

export const presetValueMin = [
  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
  [ 0, 4, 0, 0, 0, 0, 0, 0, 0, 0 ]
];

export const presetValueMax = [
  [ 255, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
  [ 255, 255, 255, 0, 0, 0, 0, 0, 0, 0 ],
  [ 255, 32, 255, 255, 0, 0, 0, 0, 0, 0 ]
];

export const presetValueDefaults = [
  [ 125, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],
  [ 125, 120, 255, 0, 0, 0, 0, 0, 0, 0 ],
  [ 85, 16, 120, 160, 0, 0, 0, 0, 0, 0 ]
];
