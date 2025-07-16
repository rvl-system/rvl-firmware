/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

This file is part of RVL Firmware.

RVL Firmware is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RVL Firmware is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RVL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "./controls/controls.hpp"
#include "./state.hpp"
#include <Arduino.h>
#include <rvl-wifi.hpp>

namespace Controls {

#define NETWORK_FLASH_RATE 500

bool connectedLEDState = false;
uint32_t nextConnectedLEDFlashTime = 0;

void init() {
#ifdef HAS_CONTROLS
  pinMode(CONTROL_DIGIT_1, INPUT);
  pinMode(CONTROL_DIGIT_2, INPUT);
  pinMode(CONTROL_DIGIT_3, INPUT);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(BRIGHTNESS_PIN, ANALOG);
  rvl::info("Controls initialized");
#endif
}

void loop() {
#ifdef HAS_CONTROLS
  int brightness = analogRead(BRIGHTNESS_PIN);
  if (rvl::getBrightness() != brightness) {
    rvl::info("Brightness changed to %d", brightness);
  }

  int digit1 = digitalRead(CONTROL_DIGIT_1);
  int digit2 = digitalRead(CONTROL_DIGIT_2);
  int digit3 = digitalRead(CONTROL_DIGIT_3);
  int channel = digit1 * 4 + digit2 * 2 + digit3;
  if (rvl::getChannel() != channel) {
    rvl::info("Channel changed to %d", channel);
    rvl::setChannel(channel);
  }
#endif
}

} // namespace Controls
