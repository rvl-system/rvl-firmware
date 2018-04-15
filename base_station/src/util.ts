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

import { Preset, Control, IdleState } from './codes';

// Message Type helpers

export interface IMessageTypeValues {
  SetBrightness: any;
  SetPreset: any;
  SetValue: any;
}
export function getValueByMessageType(messageType: MessageType, values: IMessageTypeValues): any {
  switch (messageType) {
    case MessageType.SetBrightness:
      return values.SetBrightness;
    case MessageType.SetPreset:
      return values.SetPreset;
    case MessageType.SetValue:
      return values.SetValue;
    default:
      throw new Error(`Unknown message type ${messageType}`);
  }
}

export interface IMessageTypeOperations {
  SetBrightness: () => any;
  SetPreset: () => any;
  SetValue: () => any;
}
export function runOperationByMessageType(messageType: MessageType, operations: IMessageTypeOperations): any {
  return getValueByMessageType(messageType, operations)();
}
export function runOperationForEachMessageType(operations: IMessageTypeOperations): void {
  operations.SetBrightness();
  operations.SetPreset();
  operations.SetValue();
}

// Preset helpers

export interface IPresetValues {
  Fade: any;
  Pulse: any;
}
export function getValueByPreset(preset: Preset, values: IPresetValues): any {
  switch (preset) {
    case Preset.Fade:
      return values.Fade;
    case Preset.Pulse:
      return values.Pulse;
    default:
      throw new Error(`Unknown preset ${preset}`);
  }
}

export interface IPresetOperations {
  Fade: () => any;
  Pulse: () => any;
}
export function runOperationByPreset(preset: Preset, operations: IPresetOperations): any {
  return getValueByPreset(preset, operations)();
}
export function runOperationForEachPreset(operations: IPresetOperations): void {
  operations.Fade();
  operations.Pulse();
}

// Fade value helpers

export interface IFadeValueValues {
  Rate: any;
}
export function getValueByFadeValue(code: FadeValue, values: IFadeValueValues): any {
  switch (code) {
    case FadeValue.Rate:
      return values.Rate;
    default:
      throw new Error(`Unknown Fade value ${code}`);
  }
}

export interface IFadeValueOperations {
  Rate: () => any;
}
export function runOperationByFadeValue(code: FadeValue, operations: IFadeValueOperations): any {
  return getValueByFadeValue(code, operations)();
}
export function runOperationForEachFadeValue(operations: IFadeValueOperations): void {
  operations.Rate();
}

// Pulse value helpers

export interface IPulseValueValues {
  Rate: any;
  Hue: any;
  Saturation: any;
}
export function getValueByPulseValue(code: PulseValue, values: IPulseValueValues): any {
  switch (code) {
    case PulseValue.Rate:
      return values.Rate;
    case PulseValue.Hue:
      return values.Hue;
    case PulseValue.Saturation:
      return values.Saturation;
    default:
      throw new Error(`Unknown Fade value ${code}`);
  }
}

export interface IPulseValueOperations {
  Rate: () => any;
  Hue: () => any;
  Saturation: () => any;
}
export function runOperationByPulseValue(code: PulseValue, operations: IPulseValueOperations): any {
  return getValueByPulseValue(code, operations)();
}
export function runOperationForEachPulseValue(operations: IPulseValueOperations): void {
  operations.Rate();
  operations.Hue();
  operations.Saturation();
}

// Control helpers

export interface IControlValues {
  Brightness: any;
  Preset: any;
  Value1: any;
  Value2: any;
  Value3: any;
  None: any;
}
export function getValueByControl(control: Control, values: IControlValues): any {
  switch (control) {
    case Control.Brightness:
      return values.Brightness;
    case Control.Preset:
      return values.Preset;
    case Control.Value1:
      return values.Value1;
    case Control.Value2:
      return values.Value2;
    case Control.Value3:
      return values.Value3;
    case Control.None:
      return values.None;
    default:
      throw new Error(`Unknown control ${control}`);
  }
}

export interface IControlOperations {
  Brightness: () => any;
  Preset: () => any;
  Value1: () => any;
  Value2: () => any;
  Value3: () => any;
  None: () => any;
}
export function runOperationByControl(control: Control, operations: IControlOperations): any {
  return getValueByControl(control, operations)();
}
export function runOperationForEachControl(operations: IControlOperations): void {
  operations.Brightness();
  operations.Preset();
  operations.Value1();
  operations.Value2();
  operations.Value3();
  operations.None();
}

// Idle State helpers

export interface IIdleValues {
  Active: any;
  ShallowIdle: any;
  DeepIdle: any;
}
export function getValueByIdleState(idleState: IdleState, values: IIdleValues) {
  switch (idleState) {
    case IdleState.Active:
      return values.Active;
    case IdleState.ShallowIdle:
      return values.ShallowIdle;
    case IdleState.DeepIdle:
      return values.DeepIdle;
    default:
      throw new Error(`Unknown idle state ${idleState}`);
  }
}

export interface IIdleOperations {
  Active: () => any;
  ShallowIdle: () => any;
  DeepIdle: () => any;
}
export function runOperationByIdleState(code: IdleState, operations: IIdleOperations): any {
  return getValueByIdleState(code, operations)();
}
export function runOperationForEachIdleState(operations: IIdleOperations): void {
  operations.Active();
  operations.ShallowIdle();
  operations.DeepIdle();
}
