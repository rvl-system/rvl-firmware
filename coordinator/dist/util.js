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
'use strict';
var codes_1 = require("./codes");
function getValueByMessageType(messageType, values) {
    switch (messageType) {
        case codes_1.MessageType.SetBrightness:
            return values.SetBrightness;
        case codes_1.MessageType.SetPreset:
            return values.SetPreset;
        case codes_1.MessageType.SetValue:
            return values.SetValue;
        default:
            throw new Error("Unknown message type " + messageType);
    }
}
exports.getValueByMessageType = getValueByMessageType;
function runOperationByMessageType(messageType, operations) {
    return getValueByMessageType(messageType, operations)();
}
exports.runOperationByMessageType = runOperationByMessageType;
function runOperationForEachMessageType(operations) {
    operations.SetBrightness();
    operations.SetPreset();
    operations.SetValue();
}
exports.runOperationForEachMessageType = runOperationForEachMessageType;
function getValueByPreset(preset, values) {
    switch (preset) {
        case codes_1.Preset.Fade:
            return values.Fade;
        case codes_1.Preset.Pulse:
            return values.Pulse;
        default:
            throw new Error("Unknown preset " + preset);
    }
}
exports.getValueByPreset = getValueByPreset;
function runOperationByPreset(preset, operations) {
    return getValueByPreset(preset, operations)();
}
exports.runOperationByPreset = runOperationByPreset;
function runOperationForEachPreset(operations) {
    operations.Fade();
    operations.Pulse();
}
exports.runOperationForEachPreset = runOperationForEachPreset;
function getValueByFadeValue(code, values) {
    switch (code) {
        case codes_1.FadeValue.Rate:
            return values.Rate;
        default:
            throw new Error("Unknown Fade value " + code);
    }
}
exports.getValueByFadeValue = getValueByFadeValue;
function runOperationByFadeValue(code, operations) {
    return getValueByFadeValue(code, operations)();
}
exports.runOperationByFadeValue = runOperationByFadeValue;
function runOperationForEachFadeValue(operations) {
    operations.Rate();
}
exports.runOperationForEachFadeValue = runOperationForEachFadeValue;
function getValueByPulseValue(code, values) {
    switch (code) {
        case codes_1.PulseValue.Rate:
            return values.Rate;
        case codes_1.PulseValue.Hue:
            return values.Hue;
        case codes_1.PulseValue.Saturation:
            return values.Saturation;
        default:
            throw new Error("Unknown Fade value " + code);
    }
}
exports.getValueByPulseValue = getValueByPulseValue;
function runOperationByPulseValue(code, operations) {
    return getValueByPulseValue(code, operations)();
}
exports.runOperationByPulseValue = runOperationByPulseValue;
function runOperationForEachPulseValue(operations) {
    operations.Rate();
    operations.Hue();
    operations.Saturation();
}
exports.runOperationForEachPulseValue = runOperationForEachPulseValue;
function getValueByControl(control, values) {
    switch (control) {
        case codes_1.Control.Brightness:
            return values.Brightness;
        case codes_1.Control.Preset:
            return values.Preset;
        case codes_1.Control.Value1:
            return values.Value1;
        case codes_1.Control.Value2:
            return values.Value2;
        case codes_1.Control.Value3:
            return values.Value3;
        case codes_1.Control.None:
            return values.None;
        default:
            throw new Error("Unknown control " + control);
    }
}
exports.getValueByControl = getValueByControl;
function runOperationByControl(control, operations) {
    return getValueByControl(control, operations)();
}
exports.runOperationByControl = runOperationByControl;
function runOperationForEachControl(operations) {
    operations.Brightness();
    operations.Preset();
    operations.Value1();
    operations.Value2();
    operations.Value3();
    operations.None();
}
exports.runOperationForEachControl = runOperationForEachControl;
