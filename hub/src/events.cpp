/*
Copyright (c) 2017 Bryan Hughes <bryan@nebri.us>

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
#include "./codes.h"
#include "./screen.h"
#include "./messaging/messaging_server.h"
#include "./events.h"
#include "./lights.h"

namespace Events {

void emitControlEvent(byte currentControl) {
  Serial.print("Setting control ");
  Serial.println(currentControl);

  Screen::update();
}

void emitValueEvent(byte preset, byte code, byte newValue) {
  Serial.print("Setting preset ");
  Serial.print(preset);
  Serial.print(" code ");
  Serial.print(code);
  Serial.print(" to value ");
  Serial.println(newValue);

  Screen::update();
  MessagingServer::update();
  Lights::update();
}

void emitBrightnessEvent(byte brightness) {
  Serial.print("Setting brightness to ");
  Serial.println(brightness);

  Screen::update();
  MessagingServer::update();
  Lights::update();
}

void emitPresetEvent(byte preset) {
  Serial.print("Setting preset ");
  Serial.println(preset);

  Screen::update();
  MessagingServer::update();
  Lights::update();
}

void emitClientEvent(byte numClients) {
  Serial.print("Setting num clients to ");
  Serial.println(numClients);

  Screen::update();
  MessagingServer::update();
  Lights::update();
}

void emitIdleEvent(Codes::IdleState::IdleState idleState) {
  Serial.print("Setting idle state to ");
  Serial.println(idleState);

  Screen::updateIdleState(idleState);
}

}  // namespace Events
