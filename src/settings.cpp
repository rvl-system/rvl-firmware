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
#include <rvl.hpp>
#include <rvl/config.hpp>

namespace Settings {

#ifdef ESP32
Preferences preferences; // NOLINT
#endif

#define MAX_SSID_LENGTH 32
#define MAX_PASSPHRASE_LENGTH 64

// Flash writes stall both cores, so settings are written once a value has been
// stable for this long instead of on every change, and never in the path that
// answers a packet: a write there loses the reply about one time in five
#define SETTING_WRITE_DELAY 1000
// Every key the wire sets, and a few of the UI's: a key that finds the queue
// full is written at once, which in the packet path loses the reply
#define MAX_PENDING_SETTINGS (NUM_PROPERTIES + 3)
// The nickname is the one string setting
#define MAX_STRING_SETTING_LENGTH NICKNAME_LENGTH

enum class SettingKind : uint8_t { Byte, Short, String };

struct PendingSetting {
  const char* key;
  uint32_t lastChanged;
  uint16_t value;
  SettingKind kind;
  char text[MAX_STRING_SETTING_LENGTH + 1];
};

// Only ever touched from the background task
PendingSetting pendingSettings[MAX_PENDING_SETTINGS];
uint8_t numPendingSettings = 0;

char ssid[MAX_SSID_LENGTH];
char passphrase[MAX_PASSPHRASE_LENGTH];

uint8_t brightness = 0;
uint8_t channel = 0;
rvl::DeviceMode mode = rvl::DeviceMode::Receiver;
uint16_t pixelCount = 0;
rvl::ColorOrder colorOrder = DEFAULT_COLOR_ORDER;
uint8_t statusLedBrightness = 0;
char nickname[NICKNAME_LENGTH + 1] = "";
uint16_t currentLimit = 0;

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

void updatePixelCount() {
  uint16_t newPixelCount = rvl::getPixelCount();
  if (newPixelCount != pixelCount) {
    pixelCount = newPixelCount;
    setSetting16("pixel-count", pixelCount);
  }
}

void updateColorOrder() {
  rvl::ColorOrder newColorOrder = rvl::getColorOrder();
  if (newColorOrder != colorOrder) {
    colorOrder = newColorOrder;
    setSetting("color-order", static_cast<uint8_t>(colorOrder));
  }
}

void updateStatusLedBrightness() {
  uint8_t newStatusLedBrightness = rvl::getStatusLedBrightness();
  if (newStatusLedBrightness != statusLedBrightness) {
    statusLedBrightness = newStatusLedBrightness;
    setSetting("led-brightness", statusLedBrightness);
  }
}

void updateNickname() {
  const char* newNickname = rvl::getNickname();
  if (strcmp(newNickname, nickname) != 0) {
    strlcpy(nickname, newNickname, sizeof(nickname));
    setStringSetting("nickname", nickname);
  }
}

void updateCurrentLimit() {
  uint16_t newCurrentLimit = rvl::getCurrentLimit();
  if (newCurrentLimit != currentLimit) {
    currentLimit = newCurrentLimit;
    setSetting16("current-limit", currentLimit);
  }
}

void init() {
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

  pixelCount = getSetting16("pixel-count", LED_MAX_PIXELS);
  rvl::setPixelCount(pixelCount);
  rvl::on(EVENT_PIXEL_COUNT_UPDATED, updatePixelCount);

  colorOrder = static_cast<rvl::ColorOrder>(
      getSetting("color-order", static_cast<uint8_t>(DEFAULT_COLOR_ORDER)));
  rvl::setColorOrder(colorOrder);
  rvl::on(EVENT_COLOR_ORDER_UPDATED, updateColorOrder);

  statusLedBrightness =
      getSetting("led-brightness", DEFAULT_STATUS_LED_BRIGHTNESS);
  rvl::setStatusLedBrightness(statusLedBrightness);
  rvl::on(EVENT_STATUS_LED_BRIGHTNESS_UPDATED, updateStatusLedBrightness);

  getStringSetting("nickname", nickname, sizeof(nickname));
  rvl::setNickname(nickname);
  rvl::on(EVENT_NICKNAME_UPDATED, updateNickname);

  currentLimit = getSetting16("current-limit", DEFAULT_CURRENT_LIMIT);
  rvl::setCurrentLimit(currentLimit);
  rvl::on(EVENT_CURRENT_LIMIT_UPDATED, updateCurrentLimit);
}

char* getWiFiSSID() {
  return ssid;
}
void setWiFiSSID(const char* newSSID) {
  strlcpy(ssid, newSSID, MAX_SSID_LENGTH);
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
  strlcpy(passphrase, newPassphrase, MAX_PASSPHRASE_LENGTH);
#ifdef ESP32
  preferences.begin("rvl", false);
  preferences.putString("wifi-passphrase", passphrase);
  preferences.end();
#endif
}

void writeSetting(const PendingSetting& setting) {
#ifdef ESP32
  preferences.begin("rvl", false);
  switch (setting.kind) {
  case SettingKind::Byte:
    preferences.putUChar(setting.key, setting.value);
    break;
  case SettingKind::Short:
    preferences.putUShort(setting.key, setting.value);
    break;
  case SettingKind::String:
    preferences.putString(setting.key, setting.text);
    break;
  }
  preferences.end();
#endif
}

PendingSetting* findPendingSetting(const char* key) {
  for (uint8_t i = 0; i < numPendingSettings; i++) {
    if (strcmp(pendingSettings[i].key, key) == 0) {
      return &pendingSettings[i];
    }
  }
  return nullptr;
}

// Replaces a pending write of the same key, and writes at once only when the
// queue is full
void queueSetting(const PendingSetting& setting) {
  PendingSetting* pending = findPendingSetting(setting.key);
  if (pending == nullptr) {
    if (numPendingSettings == MAX_PENDING_SETTINGS) {
      writeSetting(setting);
      return;
    }
    pending = &pendingSettings[numPendingSettings++];
  }
  *pending = setting;
}

uint8_t getSetting(const char* key, uint8_t defaultValue) {
  PendingSetting* pending = findPendingSetting(key);
  if (pending != nullptr) {
    return pending->value;
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
  PendingSetting setting = {key, millis(), value, SettingKind::Byte, ""};
  queueSetting(setting);
}

uint16_t getSetting16(const char* key, uint16_t defaultValue) {
  PendingSetting* pending = findPendingSetting(key);
  if (pending != nullptr) {
    return pending->value;
  }
#ifdef ESP32
  preferences.begin("rvl", false);
  uint16_t value = preferences.getUShort(key, defaultValue);
  preferences.end();
  return value;
#else
  return defaultValue;
#endif
}

void setSetting16(const char* key, uint16_t value) {
  PendingSetting setting = {key, millis(), value, SettingKind::Short, ""};
  queueSetting(setting);
}

void getStringSetting(const char* key, char* buffer, size_t size) {
  PendingSetting* pending = findPendingSetting(key);
  if (pending != nullptr) {
    strlcpy(buffer, pending->text, size);
    return;
  }
#ifdef ESP32
  preferences.begin("rvl", false);
  preferences.getString(key, buffer, size);
  preferences.end();
#endif
}

void setStringSetting(const char* key, const char* value) {
  PendingSetting setting = {key, millis(), 0, SettingKind::String, ""};
  strlcpy(setting.text, value, sizeof(setting.text));
  queueSetting(setting);
}

// Writes at most one setting per pass, so a burst of changes never turns into a
// burst of flash writes
void loop() {
  uint32_t now = millis();
  for (uint8_t i = 0; i < numPendingSettings; i++) {
    if (now - pendingSettings[i].lastChanged < SETTING_WRITE_DELAY) {
      continue;
    }
    writeSetting(pendingSettings[i]);
    pendingSettings[i] = pendingSettings[numPendingSettings - 1];
    numPendingSettings--;
    return;
  }
}

} // namespace Settings
