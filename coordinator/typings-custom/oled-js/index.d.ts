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

declare module 'oled-js' {

  import * as Five from 'johnny-five';

  interface IOledOptions {
    height?: number;
    width?: number;
    address?: number;
    microview?: boolean;
    slavePin?: number;
    resetPin?: number;
  }

  class Oled {
    constructor(board: Five.Board, five: any, opts: IOledOptions);
    clearDisplay(updateImmediately?: boolean): void;
    dimDisplay(dim: boolean): void;
    invertDisplay(invert: boolean): void;
    turnOffDisplay(): void;
    turnOnDisplay(): void;
    drawPixel(pixels: [ number, number, number ][], updateImmediately?: boolean): void;
    drawLine(x0: number, y0: number, x1: number, y1: number, color: number, updateImmediately?: boolean): void;
    fillRect(x0: number, y0: number, x1: number, y1: number, color: number, updateImmediately?: boolean): void;
    drawRect(x0: number, y0: number, x1: number, y1: number, color: number, updateImmediately?: boolean): void;
    drawCircle(x: number, y: number, r: number, color: number, updateImmediately?: boolean): void;
    drawBitmap(data: number[][], updateImmediately?: boolean): void;
    startScroll(direction: 'left' | 'right', start: number, stop: number): void;
    stopScroll(): void;
    setCursor(x: number, y: number): void;
    writeString(font: any, size: number, text: string, color: number, wrapping: boolean, int: number, updateImmediately?: boolean): void;
    update(): void;
  }

  export = Oled;
}
