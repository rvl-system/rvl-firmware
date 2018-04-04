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
Object.defineProperty(exports, "__esModule", { value: true });
const events_1 = require("events");
const codes_1 = require("./codes");
const util_1 = require("./util");
const BRIGHTNESS_STEP = 7;
const MAX_BRIGHTNESS = 255;
const VALUE_STEP = 4;
const MAX_VALUE = 255;
const DIM_TIMEOUT = 10000;
const OFF_TIMEOUT = 5000;
const settings = {
    preset: 0,
    brightness: 20,
    fadeValues: {
        rate: 125
    },
    pulseValues: {
        rate: 125,
        hue: 0,
        saturation: 255
    },
    currentControl: 0,
    numClients: 0,
    idleState: codes_1.IdleState.DeepIdle
};
var Direction;
(function (Direction) {
    Direction[Direction["Up"] = 0] = "Up";
    Direction[Direction["Down"] = 1] = "Down";
})(Direction || (Direction = {}));
class State extends events_1.EventEmitter {
    nextControl() {
        const numPresetValues = util_1.getValueByPreset(settings.preset, {
            Fade: numFadeValues,
            Pulse: numPulseValues
        });
        settings.currentControl++;
        if (settings.currentControl === 3 + numPresetValues) {
            settings.currentControl = 0;
        }
        this.emit('control', settings.currentControl);
    }
    controlUp(steps) {
        util_1.runOperationByControl(settings.currentControl, {
            Brightness: () => {
                if (settings.brightness < MAX_BRIGHTNESS) {
                    settings.brightness += BRIGHTNESS_STEP * steps;
                    if (settings.brightness > MAX_BRIGHTNESS) {
                        settings.brightness = MAX_BRIGHTNESS;
                    }
                    this.emit('brightness', settings.brightness);
                }
            },
            Preset: () => {
                settings.preset++;
                if (settings.preset === numPresets) {
                    settings.preset = 0;
                }
                this.emit('preset', settings.preset);
            },
            Value1: () => this.handleValueChange(0, Direction.Up, steps),
            Value2: () => this.handleValueChange(1, Direction.Up, steps),
            Value3: () => this.handleValueChange(2, Direction.Up, steps),
            None: () => {
                // Do nothing
            }
        });
    }
    controlDown(steps) {
        util_1.runOperationByControl(settings.currentControl, {
            Brightness: () => {
                if (settings.brightness > 0) {
                    settings.brightness -= BRIGHTNESS_STEP * steps;
                    if (settings.brightness < 0) {
                        settings.brightness = 0;
                    }
                    this.emit('brightness', settings.brightness);
                }
            },
            Preset: () => {
                settings.preset--;
                if (settings.preset < 0) {
                    settings.preset = numPresets - 1;
                }
                this.emit('preset', settings.preset);
            },
            Value1: () => this.handleValueChange(0, Direction.Down, steps),
            Value2: () => this.handleValueChange(1, Direction.Down, steps),
            Value3: () => this.handleValueChange(2, Direction.Down, steps),
            None: () => {
                // Do nothing
            }
        });
    }
    clientConnected() {
        settings.numClients++;
        this.emit('client', settings.numClients);
    }
    clientDisconnected() {
        settings.numClients--;
        this.emit('client', settings.numClients);
    }
    getSettings() {
        return settings;
    }
    setActive() {
        clearTimeout(this.screenTimeout);
        settings.idleState = codes_1.IdleState.Active;
        this.emit('idle', settings.idleState);
    }
    setIdling() {
        this.screenTimeout = setTimeout(() => {
            settings.idleState = codes_1.IdleState.ShallowIdle;
            this.emit('idle', settings.idleState);
            this.screenTimeout = setTimeout(() => {
                settings.idleState = codes_1.IdleState.DeepIdle;
                this.emit('idle', settings.idleState);
            }, OFF_TIMEOUT);
        }, DIM_TIMEOUT);
    }
    handleValueChange(code, direction, steps) {
        function calculateNewValue(value) {
            switch (direction) {
                case Direction.Up:
                    value += VALUE_STEP * steps;
                    if (value > MAX_VALUE) {
                        value = MAX_VALUE;
                    }
                    return value;
                case Direction.Down:
                    value -= VALUE_STEP * steps;
                    if (value < 0) {
                        value = 0;
                    }
                    return value;
                default:
                    throw new Error(`Internal error: Unknown direction ${direction}`);
            }
        }
        const newValue = util_1.runOperationByPreset(settings.preset, {
            Fade: () => util_1.runOperationByFadeValue(code, {
                Rate: () => settings.fadeValues.rate = calculateNewValue(settings.fadeValues.rate)
            }),
            Pulse: () => util_1.runOperationByPulseValue(code, {
                Hue: () => settings.pulseValues.hue = calculateNewValue(settings.pulseValues.hue),
                Rate: () => settings.pulseValues.rate = calculateNewValue(settings.pulseValues.rate),
                Saturation: () => settings.pulseValues.saturation = calculateNewValue(settings.pulseValues.saturation)
            })
        });
        this.emit('value', {
            preset: settings.preset,
            code,
            value: newValue
        });
    }
}
exports.State = State;
exports.default = new State();
//# sourceMappingURL=state.js.map