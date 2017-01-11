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
import {
  numPresets,
  FadeValue, numFadeValues,
  PulseValue, numPulseValues
} from './codes';
import { getValueByPreset, runOperationByPreset, runOperationByFadeValue, runOperationByPulseValue, runOperationByControl } from './util';

const BRIGHTNESS_STEP = 7;
const MAX_BRIGHTNESS = 255;
const VALUE_STEP = 4;
const MAX_VALUE = 255;

const settings = {
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

enum Direction {
  Up,
  Down
}

export class State extends EventEmitter {

  public nextControl() {

    const numPresetValues = getValueByPreset(settings.preset, {
      Fade: numFadeValues,
      Pulse: numPulseValues
    });

    settings.currentControl++;
    if (settings.currentControl === 2 + numPresetValues) {
      settings.currentControl = 0;
    }

    this.emit('control', settings.currentControl);
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
          value += VALUE_STEP * steps;
          if (value > MAX_VALUE) {
            value = MAX_VALUE;
          }
          return value;
        default:
          throw new Error(`Internal error: Unknown direction ${direction}`);
      }
    }

    const newValue = runOperationByPreset(settings.preset, {
      Fade: () => runOperationByFadeValue(<FadeValue>code, {
        Rate: () => settings.fadeValues.rate = calculateNewValue(settings.fadeValues.rate)
      }),
      Pulse: () => runOperationByPulseValue(<PulseValue>code, {
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
      None: () => {}
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
      None: () => {}
    });
  }

  public clientConnected() {
    settings.numClients++;
  };

  public clientDisconnected() {
    settings.numClients--;
  };

  public getSettings() {
    return settings;
  }
}

export default new State();
