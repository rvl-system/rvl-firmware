// /*
// Copyright (c) 2017 Bryan Hughes <bryan@nebri.us>

// This file is part of Raver Lights.

// Raver Lights is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Raver Lights is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Raver Lights.  If not, see <http://www.gnu.org/licenses/>.
// */

#include <Arduino.h>
#ifdef ESP32
#include <Preferences.h>
#endif
#include <algorithm>
#include "./settings.h"
#include "./config.h"
#include "./rvl.h"

namespace Settings {

#ifdef ESP32
Preferences preferences;
#endif

#define MAX_SSID_LENGTH 32
#define MAX_PASSPHRASE_LENGTH 64

char ssid[MAX_SSID_LENGTH];
char passphrase[MAX_PASSPHRASE_LENGTH];
uint16_t port;

uint8_t brightness = 0;
bool remoteBrightnessState = false;
uint8_t channel = 0;
rvl::DeviceMode mode = rvl::DeviceMode::Receiver;

void updateDeviceMode() {
  rvl::DeviceMode newMode = rvl::getDeviceMode();
  if (newMode != mode) {
    mode = newMode;
    setSetting("mode", static_cast<uint8_t>(mode));
  }
}

void updateChannel() {
  uint8_t newChannel = rvl::getChannel();
  if (newChannel != channel) {
    channel = newChannel;
    setSetting("channel", rvl::getChannel());
  }
}

void updateBrightness() {
  uint8_t newBrightness = rvl::getBrightness();
  if (newBrightness != brightness) {
    brightness = newBrightness;
    setSetting("brightness", rvl::getBrightness());
  }
}

void updateRemoteBrightnessState() {
  bool newRemoteBrightnessState = rvl::getRemoteBrightnessState();
  if (newRemoteBrightnessState != remoteBrightnessState) {
    remoteBrightnessState = newRemoteBrightnessState;
    setSetting("remote-brightness", rvl::getRemoteBrightnessState());
  }
}

void init() {
  ssid[MAX_SSID_LENGTH - 1] = 0;
  passphrase[MAX_PASSPHRASE_LENGTH - 1] = 0;

#ifdef ESP32
  if (!preferences.getBool("wifi-ssid-set", false)) {
    preferences.begin("rvl", false);
    preferences.putBool("wifi-ssid-set", true);
    preferences.end();
    setWiFiSSID(DEFAULT_WIFI_SSID);
  } else {
    preferences.begin("rvl", false);
    preferences.getString("wifi-ssid", ssid, MAX_SSID_LENGTH);
    preferences.end();
  }
#else
  setWiFiSSID(DEFAULT_WIFI_SSID);
#endif

#ifdef ESP32
  if (!preferences.getBool("wifi-ps-set", false)) {
    preferences.begin("rvl", false);
    preferences.putBool("wifi-ps-set", true);
    preferences.end();
    setWiFiPassphrase(DEFAULT_WIFI_PASSPHRASE);
  } else {
    preferences.begin("rvl", false);
    preferences.getString("wifi-passphrase", passphrase, MAX_PASSPHRASE_LENGTH);
    preferences.end();
  }
#else
  setWiFiPassphrase(DEFAULT_WIFI_PASSPHRASE);
#endif

#ifdef ESP32
  preferences.begin("rvl", false);
  port = preferences.getUShort("port", DEFAULT_WIFI_PORT);
  preferences.end();
#else
  port = DEFAULT_WIFI_PORT;
#endif

  mode = static_cast<rvl::DeviceMode>(
    getSetting("mode", static_cast<uint8_t>(rvl::DeviceMode::Receiver)));
  rvl::setDeviceMode(mode);
  rvl::on(EVENT_DEVICE_MODE_UPDATED, updateDeviceMode);

  channel = getSetting("channel", DEFAULT_CHANNEL);
  rvl::setChannel(channel);
  rvl::on(EVENT_CHANNEL_UPDATED, updateChannel);

  brightness = getSetting("brightness", DEFAULT_BRIGHTNESS);
  rvl::setBrightness(brightness);
  rvl::on(EVENT_BRIGHTNESS_UPDATED, updateBrightness);

  remoteBrightnessState = getSetting("remote-brightness", false);
  rvl::setRemoteBrightnessState(remoteBrightnessState);
  rvl::on(EVENT_REMOTE_BRIGHTNESS_UPDATED, updateRemoteBrightnessState);
}

char* getWiFiSSID() {
  return ssid;
}
void setWiFiSSID(const char* newSSID) {
  memcpy(ssid, newSSID, std::min(32, static_cast<int>(strlen(newSSID))));
#ifdef ESP32
  preferences.begin("rvl", false);
  preferences.putString("wifi-ssid", ssid);
  preferences.end();
#endif
}

char* getWiFiPassphrase() {
  return passphrase;
}
void setWiFiPassphrase(const char* newPassphrase) {
  memcpy(passphrase, newPassphrase,
    std::min(63, static_cast<int>(strlen(newPassphrase))));
#ifdef ESP32
  preferences.begin("rvl", false);
  preferences.putString("wifi-passphrase", passphrase);
  preferences.end();
#endif
}

uint16_t getPort() {
  return port;
}

void setPort(uint16_t newPort) {
  port = newPort;
#ifdef ESP32
  preferences.begin("rvl", false);
  preferences.putUShort("port", port);
  preferences.end();
#endif
}

uint8_t getSetting(const char* key, uint8_t defaultValue) {
#ifdef ESP32
  preferences.begin("rvl", false);
  uint8_t value = preferences.getUChar(key, defaultValue);
  preferences.end();
  return value;
#else
  return defaultValue;
#endif
}

void setSetting(const char* key, uint8_t value) {
#ifdef ESP32
  preferences.begin("rvl", false);
  preferences.putUChar(key, value);
  preferences.end();
#endif
}

}  // namespace Settings
