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

#ifdef HAS_STATUS_LED

#include "./status_led.hpp"
#include "./config.hpp"
#include <Arduino.h>
#include <rvl.hpp>

namespace StatusLed {

// Of 255. Full brightness is too bright to look at; a remotely configured
// level replaces this
#define STATUS_LED_BRIGHTNESS 60

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

// Full scale, scaled to a level when shown
const Color RED = {255, 0, 0};
const Color YELLOW = {255, 255, 0};
const Color PURPLE = {255, 0, 255};
const Color BLUE = {0, 0, 255};

uint8_t scaleComponent(uint8_t component, uint8_t level) {
  return static_cast<uint8_t>(component * level / 255);
}

void showColor(Color color, uint8_t level) {
  ledcWrite(STATUS_LED_R_PIN, scaleComponent(color.red, level));
  ledcWrite(STATUS_LED_G_PIN, scaleComponent(color.green, level));
  ledcWrite(STATUS_LED_B_PIN, scaleComponent(color.blue, level));
}

// A triangle wave over the period in ms, from low up to high and back
uint8_t pulse(uint32_t period, uint8_t low, uint8_t high) {
  uint32_t phase = millis() % period;
  uint32_t half = period / 2;
  uint32_t rise = phase < half ? phase : period - phase;
  return static_cast<uint8_t>(low + (high - low) * rise / half);
}

void init() {
  // 5 kHz PWM at 8 bits per color
  ledcAttach(STATUS_LED_R_PIN, 5000, 8);
  ledcAttach(STATUS_LED_G_PIN, 5000, 8);
  ledcAttach(STATUS_LED_B_PIN, 5000, 8);
  showColor(BLUE, 0);
}

// Link loss first: it is the failure the LED exists to show on a board with no
// screen, and the render states still read Current or Stale under it
void loop() {
  if (!rvl::isConnected()) {
    showColor(RED, pulse(2000, 0, STATUS_LED_BRIGHTNESS));
    return;
  }
  switch (rvl::getRenderState()) {
  case rvl::RenderState::Current:
    showColor(PURPLE, STATUS_LED_BRIGHTNESS);
    break;
  case rvl::RenderState::Stale:
    showColor(
        BLUE, pulse(4000, STATUS_LED_BRIGHTNESS / 2, STATUS_LED_BRIGHTNESS));
    break;
  case rvl::RenderState::Unknown:
    showColor(YELLOW, pulse(4000, 0, STATUS_LED_BRIGHTNESS));
    break;
  }
}

} // namespace StatusLed

#endif // HAS_STATUS_LED
