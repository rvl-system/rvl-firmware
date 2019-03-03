/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

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

#ifndef ARDUINO_PLATFORM_H_
#define ARDUINO_PLATFORM_H_

#include <Arduino.h>
#include <RaverLightsMessaging.h>

namespace ArduinoPlatform {

class SerialLogging : public RVLLoggingInterface {
 private:
  RVLLogLevel level;

 public:
  explicit SerialLogging(RVLLogLevel level);

  RVLLogLevel getLogLevel();
  void print(const char s);
  void print(const char *s);
  void println();
  void println(const char s);
  void println(const char *s);
};

class ArduinoPlatform : public RVLPlatformInterface {
 private:
  uint32_t clockOffset = 0;
  uint16_t deviceId;
  RVLWaveSettings waveSettings;
  RVLDeviceMode deviceMode = RVLDeviceMode::Receiver;

 public:
  uint16_t getDeviceId();
  void setDeviceId(uint16_t newDeviceId);

  uint32_t getLocalTime();
  uint32_t getClockOffset();
  void setClockOffset(uint32_t newOffset);

  RVLDeviceMode getDeviceMode();
  void setDeviceMode(RVLDeviceMode newMode);
  RVLWaveSettings* getWaveSettings();
  void setWaveSettings(RVLWaveSettings* newWaveSettings);
};

extern SerialLogging logging;
extern ArduinoPlatform platform;

}  // namespace ArduinoPlatform

#endif  // ARDUINO_PLATFORM_H_
