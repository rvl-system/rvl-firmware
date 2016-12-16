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

#include <Arduino.h>
#include "messaging.h"

#define RATE_PIN 0
#define COLOR_PIN 1

int rateValue = 0;
int colorValue  = 0;

void controls_setup() {
  pinMode(RATE_PIN, OUTPUT);
  pinMode(COLOR_PIN, OUTPUT);
}

void controls_loop() {
  int newRateValue = analogRead(RATE_PIN);
  int newColorValue = analogRead(COLOR_PIN);
}
