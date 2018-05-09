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

export const LOW = 0;
export const HIGH = 0;

export const WIFI_SSID = 'RaverLights';
export const WIFI_PASSPHRASE = '4&7EWf7p3h222;$+wg9]&4dN,dGh4^@}';
export const SERVER_PORT = 3000;
export const UPDATE_RATE = 33;
export const NUM_PRESETS = 3;
export const NUM_PRESET_VALUES = 10;
export const DEFAULT_PRESET = 2;
export const DEFAULT_BRIGHTNESS = 25;
export const DATA_PIN = 0;
export const CLOCK_PIN = 2;
export const NUM_PIXELS = 120;
export const LCD_ADDRESS = 0x3C;
export const LCD_SDA = 4;
export const LCD_SCL = 5;
export const LCD_WIDTH = 128;
export const LCD_HEIGHT = 64;
export const ACTIVE_CONTRAST = 255;
export const SHALLOW_IDLE_CONTRAST = 16;
export const BUTTON_NEXT = 12;
export const BUTTON_NEXT_ON = LOW;
export const BUTTON_NEXT_OFF = HIGH;
export const BUTTON_UP = 16;
export const BUTTON_UP_ON = LOW;
export const BUTTON_UP_OFF = HIGH;
export const BUTTON_DOWN = 15;
export const BUTTON_DOWN_ON = HIGH;
export const BUTTON_DOWN_OFF = LOW;
export const BUTTON_LEFT = 14;
export const BUTTON_LEFT_ON = LOW;
export const BUTTON_LEFT_OFF = HIGH;
export const BUTTON_RIGHT = 13;
export const BUTTON_RIGHT_ON = LOW;
export const BUTTON_RIGHT_OFF = HIGH;
export const BUTTON_PRESS_ENGAGE_TIME = 50;
export const BUTTON_HOLD_ENGAGE_TIME = 2000;
export const CLIENT_TIMEOUT = 5000;
export const CLIENT_SYNC_INTERVAL = 2000;
export const CLOCK_SYNC_INTERVAL = 2000;

export const SERVER_IP = '192.168.42.1';
export const GATEWAY = '192.168.42.255';
export const SUBNET = '255.255.255.0';

export enum GigglePixelPacketTypes { PaletteSharing = 1, WaveParameters = 2, RaverLights = 3 }
export enum Mode { Controller = 0, Receiver = 1, Standalone = 2 }
export enum EventType { AnimationChange, InputChange }
export enum Preset { Fade = 0, Pulse = 1, Wave = 2, Unknown = 255 }
export enum Control { None = 0, Brightness = 1, Preset = 2, Value1 = 3, Value2 = 4, Value3 = 5 }

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
