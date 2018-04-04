"use strict";
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
Object.defineProperty(exports, "__esModule", { value: true });
exports.WIFI_SSID = 'RaverLights';
exports.WIFI_PASSPHRASE = '4&7EWf7p3h222;$+wg9]&4dN,dGh4^@}';
exports.SERVER_PORT = 3000;
exports.UPDATE_RATE = 33;
exports.NUM_PRESETS = 3;
exports.NUM_PRESET_VALUES = 10;
exports.DEFAULT_PRESET = 2;
exports.DEFAULT_BRIGHTNESS = 25;
exports.SERVER_IP = '192.168.42.1';
exports.GATEWAY = '192.168.42.255';
exports.SUBNET = '255.255.255.0';
var Preset;
(function (Preset) {
    Preset[Preset["Fade"] = 0] = "Fade";
    Preset[Preset["Pulse"] = 1] = "Pulse";
    Preset[Preset["Wave"] = 2] = "Wave";
    Preset[Preset["Unknown"] = 255] = "Unknown";
})(Preset = exports.Preset || (exports.Preset = {}));
var Control;
(function (Control) {
    Control[Control["None"] = 0] = "None";
    Control[Control["Brightness"] = 1] = "Brightness";
    Control[Control["Preset"] = 2] = "Preset";
    Control[Control["Value1"] = 3] = "Value1";
    Control[Control["Value2"] = 4] = "Value2";
    Control[Control["Value3"] = 5] = "Value3";
})(Control = exports.Control || (exports.Control = {}));
var IdleState;
(function (IdleState) {
    IdleState[IdleState["Active"] = 0] = "Active";
    IdleState[IdleState["ShallowIdle"] = 1] = "ShallowIdle";
    IdleState[IdleState["DeepIdle"] = 2] = "DeepIdle";
})(IdleState = exports.IdleState || (exports.IdleState = {}));
exports.presetValueLabels = [
    ['RATE', null, null, null, null, null, null, null, null, null],
    ['RATE', 'HUE', 'SATR', null, null, null, null, null, null, null],
    ['RATE', 'SPAC', 'HUEF', 'HUEB', null, null, null, null, null, null]
];
exports.presetValueMin = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 4, 0, 0, 0, 0, 0, 0, 0, 0]
];
exports.presetValueMax = [
    [255, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [255, 255, 255, 0, 0, 0, 0, 0, 0, 0],
    [255, 32, 255, 255, 0, 0, 0, 0, 0, 0]
];
exports.presetValueDefaults = [
    [125, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [125, 120, 255, 0, 0, 0, 0, 0, 0, 0],
    [85, 16, 120, 160, 0, 0, 0, 0, 0, 0]
];
//# sourceMappingURL=codes.js.map