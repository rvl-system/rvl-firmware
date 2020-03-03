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

// #include <Arduino.h>
// #include <algorithm>
#include "./settings.h"

// namespace Settings {

// Preferences preferences;

// #define MAX_SSID_LENGTH 32
// #define MAX_PASSPHRASE_LENGTH 64

// char ssid[MAX_SSID_LENGTH];
// char passphrase[MAX_PASSPHRASE_LENGTH];

// void onModeChanged(RVLDeviceMode newMode) {
//   Event::emit(Codes::EventType::ModeChange);
// }

// // Do I need an on channel changed?

// void onPowerStateChanged(bool powerState) {
//   Event::emit(Codes::EventType::PowerStateChange);
// }

// void onBrightnessChanged(uint8_t brightness) {
//   Event::emit(Codes::EventType::BrightnessChange);
// }

// void init() {
//   ssid[MAX_SSID_LENGTH - 1] = 0;
//   passphrase[MAX_PASSPHRASE_LENGTH - 1] = 0;

//   preferences.begin("rvl", false);

//   if (!preferences.getBool("wifi-ssid-set", false)) {
//     preferences.setBool("wifi-ssid-set", true);
//     setWiFiSSID(DEFAULT_WIFI_SSID);
//   } else {
//     preferences.getString("wifi-ssid", ssid, MAX_SSID_LENGTH);
//   }

//   if (!preferences.getBool("wifi-passphrase-set", false)) {
//     preferences.setBool("wifi-passphrase-set", true);
//     setWiFiSSID(DEFAULT_WIFI_PASSPHRASE);
//   } else {
//     preferences.getString("wifi-passphrase", ssid, MAX_PASSPHRASE_LENGTH);
//   }

//   if (!preferences.getBool("mode-set", false)) {
//     preferences.setBool("mode-set", true);
//     setMode(RVLDeviceMode::Receiver);
//   } else {
//     RVLSetMode(preferences.getUChar("mode"));
//   }

//   if (!preferences.getBool("channel-set", false)) {
//     preferences.setBool("channel-set", true);
//     setChannel(DEFAULT_CHANNEL);
//   } else {
//     RVLSetChannel(preferences.getUChar("channel"));
//   }

//   if (!preferences.getBool("powerState-set", false)) {
//     preferences.setBool("powerState-set", true);
//     setPowerState(true);
//   } else {
//     RVLSetPowerState(preferences.getBool("powerState"));
//   }

//   if (!preferences.getBool("brightness-set", false)) {
//     preferences.setBool("brightness-set", true);
//     brightness = (DEFAULT_BRIGHTNESS * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / 16) + MIN_BRIGHTNESS;
//   } else {
//     brightness = preferences.getUChar("brightness");
//   }

//   preferences.end();

//   RVLOnModeChanged(onModeChanged);
//   RVLOnPowerStateChanged(onPowerStateChanged);
// #ifdef REMOTE_BRIGHTNESS
//   RVLSetBrightness(brightness);
//   RVLOnBrightnessChanged(onBrightnessChanged);
// #endif
// }

// char* getWiFiSSID() {
//   return ssid;
// }
// void setWiFiSSID(char* newSSID) {
//   memcpy(ssid, newSSID, std::min(32, strlen(newSSID)));
//   preferences.begin("rvl", false);
//   preferences.putString("wifi-ssid", ssid);
//   preferences.end();
//   ESP.restart();
// }

// char* getWiFiPassphrase() {
//   return passphrase;
// }
// void setWiFiPassphrase(char* newPassphrase) {
//   memcpy(passphrase, newPassphrase, std::min(63, strlen(newPassphrase)));
//   preferences.begin("rvl", false);
//   preferences.putString("wifi-passphrase", passphrase);
//   preferences.end();
// }

// RVLDeviceMode getMode() {
//   return RVLGetMode();
// }
// void setMode(RVLDeviceMode mode) {
//   RVLSetMode(mode);
//   preferences.begin("rvl", false);
//   preferences.putUChar("mode", mode);
//   preferences.end();
// }

// uint8_t getChannel() {
//   return RVLGetChannel();
// }
// void setChannel(uint8_t channel) {
//   RVLSetChannel(channel);
//   preferences.begin("rvl", false);
//   preferences.putUChar("channel", channel);
//   preferences.end();
// }

// bool getPowerState() {
//   return RVLGetPowerState();
// }
// void setPowerState(bool powerState) {
//   RVLSetPowerState(powerState);
//   preferences.begin("rvl", false);
//   preferences.putBool("powerState", powerState);
//   preferences.end();
// }


// uint8_t getBrightness() {
// #ifdef REMOTE_BRIGHTNESS
//   brightness =  RVLGetBrightness();
//   if (brightness > MAX_BRIGHTNESS) {
//     brightness = MAX_BRIGHTNESS;
//   }
//   if (brightness < MIN_BRIGHTNESS) {
//     brightness = MIN_BRIGHTNESS;
//   }
// #endif
//   return brightness;
// }

// void setBrightness(uint8_t newBrightness) {
//   brightness = newBrightness;
// #ifdef REMOTE_BRIGHTNESS
//   RVLSetBrightness(newBrightness);
// #else
//   preferences.begin("rvl", false);
//   preferences.putUChar("brightness", brightness);
//   preferences.end();
// #endif
// }

// uint8_t getSetting(char* key) {
//   return preferences.getUChar(key);
// }
// void setSetting(char* key, uint8_t value) {
//   preferences.begin("rvl", false);
//   preferences.putUChar(key, uint8_t value);
//   preferences.end();
// }

// }  // namespace Settings
