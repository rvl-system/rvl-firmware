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
exports.LOW = 0;
exports.HIGH = 0;
exports.WIFI_SSID = 'RaverLights';
exports.WIFI_PASSPHRASE = '4&7EWf7p3h222;$+wg9]&4dN,dGh4^@}';
exports.SERVER_PORT = 4978;
exports.UPDATE_RATE = 33;
exports.NUM_PRESETS = 3;
exports.NUM_PRESET_VALUES = 10;
exports.MAX_PALETTE_ENTRIES = 10;
exports.DEFAULT_PRESET = 2;
exports.DEFAULT_BRIGHTNESS = 25;
exports.CLOCK_PIN = 0;
exports.DATA_PIN = 2;
exports.NUM_PIXELS = 180;
exports.LCD_ADDRESS = 0x3C;
exports.LCD_SDA = 4;
exports.LCD_SCL = 5;
exports.LCD_WIDTH = 128;
exports.LCD_HEIGHT = 64;
exports.ACTIVE_CONTRAST = 255;
exports.SHALLOW_IDLE_CONTRAST = 16;
exports.BUTTON_PRESS = 12;
exports.BUTTON_PRESS_ON = exports.LOW;
exports.BUTTON_PRESS_OFF = exports.HIGH;
exports.BUTTON_UP = 15;
exports.BUTTON_UP_ON = exports.LOW;
exports.BUTTON_UP_OFF = exports.HIGH;
exports.BUTTON_DOWN = 16;
exports.BUTTON_DOWN_ON = exports.HIGH;
exports.BUTTON_DOWN_OFF = exports.LOW;
exports.BUTTON_LEFT = 14;
exports.BUTTON_LEFT_ON = exports.LOW;
exports.BUTTON_LEFT_OFF = exports.HIGH;
exports.BUTTON_RIGHT = 13;
exports.BUTTON_RIGHT_ON = exports.LOW;
exports.BUTTON_RIGHT_OFF = exports.HIGH;
exports.BUTTON_PRESS_ENGAGE_TIME = 50;
exports.BUTTON_HOLD_ENGAGE_TIME = 2000;
exports.CLIENT_TIMEOUT = 5000;
exports.CLIENT_SYNC_INTERVAL = 2000;
exports.CLOCK_SYNC_INTERVAL = 2000;
exports.SERVER_IP = '192.168.42.1';
exports.GATEWAY = '192.168.42.255';
exports.SUBNET = '255.255.255.0';
var GigglePixelPacketTypes;
(function (GigglePixelPacketTypes) {
    GigglePixelPacketTypes[GigglePixelPacketTypes["Palette"] = 1] = "Palette";
    GigglePixelPacketTypes[GigglePixelPacketTypes["Wave"] = 2] = "Wave";
    GigglePixelPacketTypes[GigglePixelPacketTypes["Preset"] = 3] = "Preset";
})(GigglePixelPacketTypes = exports.GigglePixelPacketTypes || (exports.GigglePixelPacketTypes = {}));
var Mode;
(function (Mode) {
    Mode[Mode["Controller"] = 0] = "Controller";
    Mode[Mode["Receiver"] = 1] = "Receiver";
    Mode[Mode["Standalone"] = 2] = "Standalone";
})(Mode = exports.Mode || (exports.Mode = {}));
var EventType;
(function (EventType) {
    EventType[EventType["AnimationChange"] = 0] = "AnimationChange";
    EventType[EventType["UIStateChange"] = 1] = "UIStateChange";
    EventType[EventType["ConnectedStateChange"] = 2] = "ConnectedStateChange";
})(EventType = exports.EventType || (exports.EventType = {}));
var Preset;
(function (Preset) {
    Preset[Preset["Rainbow"] = 0] = "Rainbow";
    Preset[Preset["Pulse"] = 1] = "Pulse";
    Preset[Preset["Wave"] = 2] = "Wave";
    Preset[Preset["Unknown"] = 255] = "Unknown";
})(Preset = exports.Preset || (exports.Preset = {}));
var RainbowPresetValues;
(function (RainbowPresetValues) {
    RainbowPresetValues[RainbowPresetValues["Rate"] = 0] = "Rate";
})(RainbowPresetValues = exports.RainbowPresetValues || (exports.RainbowPresetValues = {}));
var PulsePresetValues;
(function (PulsePresetValues) {
    PulsePresetValues[PulsePresetValues["Rate"] = 0] = "Rate";
    PulsePresetValues[PulsePresetValues["Hue"] = 1] = "Hue";
    PulsePresetValues[PulsePresetValues["Saturation"] = 2] = "Saturation";
})(PulsePresetValues = exports.PulsePresetValues || (exports.PulsePresetValues = {}));
var WavePresetValues;
(function (WavePresetValues) {
    WavePresetValues[WavePresetValues["Rate"] = 0] = "Rate";
    WavePresetValues[WavePresetValues["ForegroundHue"] = 1] = "ForegroundHue";
    WavePresetValues[WavePresetValues["ForegroundSaturation"] = 2] = "ForegroundSaturation";
    WavePresetValues[WavePresetValues["BackgroundHue"] = 3] = "BackgroundHue";
    WavePresetValues[WavePresetValues["BackgroundSaturation"] = 4] = "BackgroundSaturation";
})(WavePresetValues = exports.WavePresetValues || (exports.WavePresetValues = {}));
var Control;
(function (Control) {
    Control[Control["None"] = 0] = "None";
    Control[Control["Brightness"] = 1] = "Brightness";
    Control[Control["Preset"] = 2] = "Preset";
    Control[Control["Value1"] = 3] = "Value1";
    Control[Control["Value2"] = 4] = "Value2";
    Control[Control["Value3"] = 5] = "Value3";
})(Control = exports.Control || (exports.Control = {}));
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
    [85, 120, 255, 160, 255, 0, 0, 0, 0, 0]
];
//# sourceMappingURL=codes.js.map