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

export const numMessageTypes = 3;
export enum MessageType {
  SetPreset = 0,
  SetValue = 1,
  SetBrightness = 2
};


export const numPresets = 2;
export enum Preset {
  Fade = 0,
  Pulse = 1
};

export const numControls = 5;
export enum Control {
  None = 0,
  Brightness = 1,
  Preset = 2,
  Value1 = 3,
  Value2 = 4,
  Value3 = 5
};

export const numFadeValues = 1;
export enum FadeValue {
  Rate = 0
}

export const numPulseValues = 3;
export enum PulseValue {
  Rate = 0,
  Hue = 1,
  Saturation = 2
};
