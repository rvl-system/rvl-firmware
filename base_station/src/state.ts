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

import { EventEmitter } from 'events';
import { IdleState } from './codes';
import {
  getValueByPreset,
  runOperationByPreset,
  runOperationByFadeValue,
  runOperationByPulseValue,
  runOperationByControl
} from './util';

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
  idleState: IdleState.DeepIdle
};

enum Direction {
  Up,
  Down
}

export class State extends EventEmitter {

  private screenTimeout: NodeJS.Timer | undefined;

  public nextControl() {

    const numPresetValues = getValueByPreset(settings.preset, {
      Fade: numFadeValues,
      Pulse: numPulseValues
    });

    settings.currentControl++;
    if (settings.currentControl === 3 + numPresetValues) {
      settings.currentControl = 0;
    }

    this.emit('control', settings.currentControl);
  }

  public controlUp(steps: number) {
    runOperationByControl(settings.currentControl, {
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

  public controlDown(steps: number) {
    runOperationByControl(settings.currentControl, {
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

  public clientConnected() {
    settings.numClients++;
    this.emit('client', settings.numClients);
  }

  public clientDisconnected() {
    settings.numClients--;
    this.emit('client', settings.numClients);
  }

  public getSettings() {
    return settings;
  }

  public setActive() {
    clearTimeout(this.screenTimeout as NodeJS.Timer);
    settings.idleState = IdleState.Active;
    this.emit('idle', settings.idleState);
  }

  public setIdling() {
    this.screenTimeout = setTimeout(() => {
      settings.idleState = IdleState.ShallowIdle;
      this.emit('idle', settings.idleState);
      this.screenTimeout = setTimeout(() => {
        settings.idleState = IdleState.DeepIdle;
        this.emit('idle', settings.idleState);
      }, OFF_TIMEOUT);
    }, DIM_TIMEOUT);
  }

  private handleValueChange(code: FadeValue | PulseValue, direction: Direction, steps: number) {

    function calculateNewValue(value: number): number {
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

    const newValue = runOperationByPreset(settings.preset, {
      Fade: () => runOperationByFadeValue(code as FadeValue, {
        Rate: () => settings.fadeValues.rate = calculateNewValue(settings.fadeValues.rate)
      }),
      Pulse: () => runOperationByPulseValue(code as PulseValue, {
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

export default new State();
