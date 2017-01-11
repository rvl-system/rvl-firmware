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
var __extends = (this && this.__extends) || function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
};
var events_1 = require("events");
var codes_1 = require("./codes");
var util_1 = require("./util");
var BRIGHTNESS_STEP = 7;
var MAX_BRIGHTNESS = 255;
var VALUE_STEP = 4;
var MAX_VALUE = 255;
var settings = {
    preset: 0,
    brightness: 10,
    fadeValues: {
        rate: 20
    },
    pulseValues: {
        rate: 20,
        hue: 0,
        saturation: 100
    },
    currentControl: 0,
    numClients: 0
};
var Direction;
(function (Direction) {
    Direction[Direction["Up"] = 0] = "Up";
    Direction[Direction["Down"] = 1] = "Down";
})(Direction || (Direction = {}));
var State = (function (_super) {
    __extends(State, _super);
    function State() {
        return _super.apply(this, arguments) || this;
    }
    State.prototype.nextControl = function () {
        var numPresetValues = util_1.getValueByPreset(settings.preset, {
            Fade: codes_1.numFadeValues,
            Pulse: codes_1.numPulseValues
        });
        settings.currentControl++;
        if (settings.currentControl === 2 + numPresetValues) {
            settings.currentControl = 0;
        }
        this.emit('control', settings.currentControl);
    };
    State.prototype.handleValueChange = function (code, direction, steps) {
        function calculateNewValue(value) {
            switch (direction) {
                case Direction.Up:
                    value += VALUE_STEP * steps;
                    if (value > MAX_VALUE) {
                        value = MAX_VALUE;
                    }
                    return value;
                case Direction.Down:
                    value += VALUE_STEP * steps;
                    if (value > MAX_VALUE) {
                        value = MAX_VALUE;
                    }
                    return value;
                default:
                    throw new Error("Internal error: Unknown direction " + direction);
            }
        }
        var newValue = util_1.runOperationByPreset(settings.preset, {
            Fade: function () { return util_1.runOperationByFadeValue(code, {
                Rate: function () { return settings.fadeValues.rate = calculateNewValue(settings.fadeValues.rate); }
            }); },
            Pulse: function () { return util_1.runOperationByPulseValue(code, {
                Hue: function () { return settings.pulseValues.hue = calculateNewValue(settings.pulseValues.hue); },
                Rate: function () { return settings.pulseValues.rate = calculateNewValue(settings.pulseValues.rate); },
                Saturation: function () { return settings.pulseValues.saturation = calculateNewValue(settings.pulseValues.saturation); }
            }); }
        });
        this.emit('value', {
            preset: settings.preset,
            code: code,
            value: newValue
        });
    };
    State.prototype.controlUp = function (steps) {
        var _this = this;
        util_1.runOperationByControl(settings.currentControl, {
            Brightness: function () {
                if (settings.brightness < MAX_BRIGHTNESS) {
                    settings.brightness += BRIGHTNESS_STEP * steps;
                    if (settings.brightness > MAX_BRIGHTNESS) {
                        settings.brightness = MAX_BRIGHTNESS;
                    }
                    _this.emit('brightness', settings.brightness);
                }
            },
            Preset: function () {
                settings.preset++;
                if (settings.preset === codes_1.numPresets) {
                    settings.preset = 0;
                }
                _this.emit('preset', settings.preset);
            },
            Value1: function () { return _this.handleValueChange(0, Direction.Up, steps); },
            Value2: function () { return _this.handleValueChange(1, Direction.Up, steps); },
            Value3: function () { return _this.handleValueChange(2, Direction.Up, steps); },
            None: function () { }
        });
    };
    State.prototype.controlDown = function (steps) {
        var _this = this;
        util_1.runOperationByControl(settings.currentControl, {
            Brightness: function () {
                if (settings.brightness > 0) {
                    settings.brightness -= BRIGHTNESS_STEP * steps;
                    if (settings.brightness < 0) {
                        settings.brightness = 0;
                    }
                    _this.emit('brightness', settings.brightness);
                }
            },
            Preset: function () {
                settings.preset--;
                if (settings.preset < 0) {
                    settings.preset = codes_1.numPresets - 1;
                }
                _this.emit('preset', settings.preset);
            },
            Value1: function () { return _this.handleValueChange(0, Direction.Down, steps); },
            Value2: function () { return _this.handleValueChange(1, Direction.Down, steps); },
            Value3: function () { return _this.handleValueChange(2, Direction.Down, steps); },
            None: function () { }
        });
    };
    State.prototype.clientConnected = function () {
        settings.numClients++;
    };
    ;
    State.prototype.clientDisconnected = function () {
        settings.numClients--;
    };
    ;
    State.prototype.getSettings = function () {
        return settings;
    };
    return State;
}(events_1.EventEmitter));
exports.State = State;
Object.defineProperty(exports, "__esModule", { value: true });
exports.default = new State();
