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
"use strict";
exports.numMessageTypes = 3;
var MessageType;
(function (MessageType) {
    MessageType[MessageType["SetPreset"] = 0] = "SetPreset";
    MessageType[MessageType["SetValue"] = 1] = "SetValue";
    MessageType[MessageType["SetBrightness"] = 2] = "SetBrightness";
})(MessageType = exports.MessageType || (exports.MessageType = {}));
;
exports.numPresets = 2;
var Preset;
(function (Preset) {
    Preset[Preset["Fade"] = 0] = "Fade";
    Preset[Preset["Pulse"] = 1] = "Pulse";
})(Preset = exports.Preset || (exports.Preset = {}));
;
exports.numControls = 5;
var Control;
(function (Control) {
    Control[Control["None"] = 0] = "None";
    Control[Control["Brightness"] = 1] = "Brightness";
    Control[Control["Preset"] = 2] = "Preset";
    Control[Control["Value1"] = 3] = "Value1";
    Control[Control["Value2"] = 4] = "Value2";
    Control[Control["Value3"] = 5] = "Value3";
})(Control = exports.Control || (exports.Control = {}));
;
exports.numFadeValues = 1;
var FadeValue;
(function (FadeValue) {
    FadeValue[FadeValue["Rate"] = 0] = "Rate";
})(FadeValue = exports.FadeValue || (exports.FadeValue = {}));
exports.numPulseValues = 3;
var PulseValue;
(function (PulseValue) {
    PulseValue[PulseValue["Rate"] = 0] = "Rate";
    PulseValue[PulseValue["Hue"] = 1] = "Hue";
    PulseValue[PulseValue["Saturation"] = 2] = "Saturation";
})(PulseValue = exports.PulseValue || (exports.PulseValue = {}));
;
