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
#include <Preferences.h>
#include <algorithm>
#include "./settings.h"
#include "./config.h"
#include "./rvl.h"

namespace Settings {

Preferences preferences;

#define MAX_SSID_LENGTH 32
#define MAX_PASSPHRASE_LENGTH 64

char ssid[MAX_SSID_LENGTH];
char passphrase[MAX_PASSPHRASE_LENGTH];
uint16_t port;

uint8_t brightness = 0;
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

void init() {
  ssid[MAX_SSID_LENGTH - 1] = 0;
  passphrase[MAX_PASSPHRASE_LENGTH - 1] = 0;

  preferences.begin("rvl", false);

  if (!preferences.getBool("wifi-ssid-set", false)) {
    preferences.putBool("wifi-ssid-set", true);
    setWiFiSSID(DEFAULT_WIFI_SSID);
  } else {
    preferences.getString("wifi-ssid", ssid, MAX_SSID_LENGTH);
  }

  if (!preferences.getBool("wifi-ps-set", false)) {
    preferences.putBool("wifi-ps-set", true);
    setWiFiPassphrase(DEFAULT_WIFI_PASSPHRASE);
  } else {
    preferences.getString("wifi-passphrase", passphrase, MAX_PASSPHRASE_LENGTH);
  }

  if (!preferences.getBool("port-set", false)) {
    preferences.putBool("port-set", true);
    setPort(DEFAULT_WIFI_PORT);
  } else {
    port = preferences.getUShort("port");
  }

  if (!preferences.getBool("mode-set", false)) {
    preferences.putBool("mode-set", true);
    setSetting("mode", static_cast<uint8_t>(rvl::DeviceMode::Receiver));
    rvl::setDeviceMode(rvl::DeviceMode::Receiver);
  } else {
    rvl::setDeviceMode(static_cast<rvl::DeviceMode>(getSetting("mode")));
  }
  rvl::on(EVENT_DEVICE_MODE_UPDATED, updateDeviceMode);

  if (!preferences.getBool("channel-set", false)) {
    preferences.putBool("channel-set", true);
    setSetting("channel", DEFAULT_CHANNEL);
    rvl::setChannel(DEFAULT_CHANNEL);
  } else {
    rvl::setChannel(getSetting("channel"));
  }
  rvl::on(EVENT_CHANNEL_UPDATED, updateChannel);

  if (!preferences.getBool("brightness-set", false)) {
    preferences.putBool("brightness-set", true);
    setSetting("brightnessr", DEFAULT_BRIGHTNESS);
    rvl::setBrightness(DEFAULT_BRIGHTNESS);
  } else {
    rvl::setBrightness(getSetting("brightness"));
  }
  rvl::on(EVENT_BRIGHTNESS_UPDATED, updateBrightness);

  // TODO(nebrius): wire this up properly so it can be changed
  rvl::setRemoteBrightnessState(false);

  preferences.end();
}

char* getWiFiSSID() {
  return ssid;
}
void setWiFiSSID(const char* newSSID) {
  memcpy(ssid, newSSID, std::min(32, static_cast<int>(strlen(newSSID))));
  preferences.begin("rvl", false);
  preferences.putString("wifi-ssid", ssid);
  preferences.end();
}

char* getWiFiPassphrase() {
  return passphrase;
}
void setWiFiPassphrase(const char* newPassphrase) {
  memcpy(passphrase, newPassphrase, std::min(63, static_cast<int>(strlen(newPassphrase))));
  preferences.begin("rvl", false);
  preferences.putString("wifi-passphrase", passphrase);
  preferences.end();
}

uint16_t getPort() {
  return port;
}

void setPort(uint16_t newPort) {
  port = newPort;
  preferences.begin("rvl", false);
  preferences.putUShort("port", port);
  preferences.end();
}

uint8_t getSetting(const char* key) {
  return preferences.getUChar(key);
}
void setSetting(const char* key, uint8_t value) {
  preferences.begin("rvl", false);
  preferences.putUChar(key, value);
  preferences.end();
}

}  // namespace Settings
