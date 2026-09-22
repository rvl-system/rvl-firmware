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

#include <Arduino.h>
#ifdef ESP32
#include <Preferences.h>
#endif
#include "./config.hpp"
#include "./settings.hpp"
#include <algorithm>
#include <rvl.hpp>

namespace Settings {

#ifdef ESP32
Preferences preferences; // NOLINT
#endif

#define MAX_SSID_LENGTH 32
#define MAX_PASSPHRASE_LENGTH 64

// Flash writes stall both cores, so settings are written once a value has been
// stable for this long instead of on every change
#define SETTING_WRITE_DELAY 1000
#define MAX_PENDING_SETTINGS 8

struct PendingSetting {
  const char* key;
  uint8_t value;
  uint32_t lastChanged;
};

// Only ever touched from the background task
PendingSetting pendingSettings[MAX_PENDING_SETTINGS];
uint8_t numPendingSettings = 0;

char ssid[MAX_SSID_LENGTH];
char passphrase[MAX_PASSPHRASE_LENGTH];

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

#ifdef ESP32
  // Preferences only reads from flash once begun, so the flag has to be read
  // inside a begin/end pair. setWiFiSSID opens its own, so it can't be called
  // from inside this one
  preferences.begin("rvl", false);
  bool ssidSet = preferences.getBool("wifi-ssid-set", false);
  if (ssidSet) {
    preferences.getString("wifi-ssid", ssid, MAX_SSID_LENGTH);
  } else {
    preferences.putBool("wifi-ssid-set", true);
  }
  preferences.end();
  if (!ssidSet) {
    setWiFiSSID(DEFAULT_WIFI_SSID);
  }
#else
  setWiFiSSID(DEFAULT_WIFI_SSID);
#endif

#ifdef ESP32
  preferences.begin("rvl", false);
  bool passphraseSet = preferences.getBool("wifi-ps-set", false);
  if (passphraseSet) {
    preferences.getString("wifi-passphrase", passphrase, MAX_PASSPHRASE_LENGTH);
  } else {
    preferences.putBool("wifi-ps-set", true);
  }
  preferences.end();
  if (!passphraseSet) {
    setWiFiPassphrase(DEFAULT_WIFI_PASSPHRASE);
  }
#else
  setWiFiPassphrase(DEFAULT_WIFI_PASSPHRASE);
#endif

  mode = static_cast<rvl::DeviceMode>(
      getSetting("mode", static_cast<uint8_t>(rvl::DeviceMode::Receiver)));
  rvl::setDeviceMode(mode);
  rvl::on(EVENT_DEVICE_MODE_UPDATED, updateDeviceMode);

  // If we have physical controls, channel and brightness are continually read
  // from the pins for them, so there's no need to save the value to storage.
#ifndef HAS_CONTROLS
  channel = getSetting("channel", DEFAULT_CHANNEL);
  rvl::setChannel(channel);
  rvl::on(EVENT_CHANNEL_UPDATED, updateChannel);

  brightness = getSetting("brightness", DEFAULT_BRIGHTNESS);
  rvl::setBrightness(brightness);
  rvl::on(EVENT_BRIGHTNESS_UPDATED, updateBrightness);
#endif
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

void writeSetting(const char* key, uint8_t value) {
#ifdef ESP32
  preferences.begin("rvl", false);
  preferences.putUChar(key, value);
  preferences.end();
#endif
}

uint8_t getSetting(const char* key, uint8_t defaultValue) {
  for (uint8_t i = 0; i < numPendingSettings; i++) {
    if (strcmp(pendingSettings[i].key, key) == 0) {
      return pendingSettings[i].value;
    }
  }
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
  for (uint8_t i = 0; i < numPendingSettings; i++) {
    if (strcmp(pendingSettings[i].key, key) == 0) {
      pendingSettings[i].value = value;
      pendingSettings[i].lastChanged = millis();
      return;
    }
  }
  if (numPendingSettings == MAX_PENDING_SETTINGS) {
    writeSetting(key, value);
    return;
  }
  pendingSettings[numPendingSettings] = {key, value, millis()};
  numPendingSettings++;
}

// Writes at most one setting per pass, so a burst of changes never turns into a
// burst of flash writes
void loop() {
  uint32_t now = millis();
  for (uint8_t i = 0; i < numPendingSettings; i++) {
    if (now - pendingSettings[i].lastChanged < SETTING_WRITE_DELAY) {
      continue;
    }
    writeSetting(pendingSettings[i].key, pendingSettings[i].value);
    pendingSettings[i] = pendingSettings[numPendingSettings - 1];
    numPendingSettings--;
    return;
  }
}

} // namespace Settings
