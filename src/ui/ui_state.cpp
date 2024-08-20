/*
Copyright (c) 2018 Bryan Hughes <bryan@nebri.us>

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

#ifdef HAS_UI

#include "./ui/ui_state.hpp"
#include "./codes.hpp"
#include "./presets/color_cycle.hpp"
#include "./presets/preset_control_set.hpp"
#include "./presets/pulse.hpp"
#include "./presets/rainbow.hpp"
#include "./presets/solid.hpp"
#include "./presets/wave.hpp"
#include "./presets/shift.hpp"
#include "./settings.hpp"
#include "./state.hpp"
#include <Arduino.h>
#include <rvl-wifi.hpp>
#include <rvl.hpp>

namespace UIState {

uint8_t currentTab1Control = 0;
uint8_t currentTab2Control = 0;
uint8_t preset;
uint8_t currentTab = 0;
uint32_t screenTimeout = 0;

#define NUM_GLOBAL_CONTROLS 3

std::vector<PresetControlSet*> presets;

uint8_t getBrightnessValue() {
  return 16 * (rvl::getBrightness() - MIN_BRIGHTNESS) /
      (MAX_BRIGHTNESS - MIN_BRIGHTNESS);
}
void updateBrightnessValue(uint8_t newValue) {
  uint16_t adjustedBrightness =
      (newValue * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / 16) + MIN_BRIGHTNESS;
  rvl::setBrightness(adjustedBrightness);
  rvl::info("Changing brightness to %d", adjustedBrightness);
  rvl::emit(Codes::EventType::AnimationChange);
}
Control::RangeControl* brightnessControl;

void updateChannelValue(uint8_t selectedValueIndex) {
  if (rvl::getChannel() != selectedValueIndex) {
    rvl::setChannel(selectedValueIndex);
  }
}
Control::ListControl* channelControl;

void updateModeValue(uint8_t selectedValueIndex) {
  if (rvl::getDeviceMode() != static_cast<rvl::DeviceMode>(selectedValueIndex))
  {
    switch (static_cast<rvl::DeviceMode>(selectedValueIndex)) {
    case rvl::DeviceMode::Controller:
      rvl::setDeviceMode(rvl::DeviceMode::Controller);
      presets[preset]->updateWave();
      break;
    case rvl::DeviceMode::Receiver:
      rvl::setDeviceMode(rvl::DeviceMode::Receiver);
      break;
    }
  }
}
Control::ListControl* modeControl;

void updatePresetValue(uint8_t selectedValueIndex) {
  if (UIState::preset != selectedValueIndex) {
    Settings::setSetting("ui-preset", selectedValueIndex);
    UIState::preset = selectedValueIndex;
    presets[preset]->updateWave();
  }
}
Control::ListControl* presetControl;

void updateRemoteBrightnessValue(uint8_t selectedValueIndex) {
  if (UIState::preset != selectedValueIndex) {
    rvl::setRemoteBrightnessState(selectedValueIndex == 1);
    if (selectedValueIndex == 1) {
      rvl::info("Enabling remote brightness");
    } else {
      rvl::info("Disabling remote brightness");
    }
    rvl::emit(Codes::EventType::AnimationChange);
  }
}
Control::ListControl* remoteBrightnessControl;

std::vector<Control::Control*> tab1Controls;
std::vector<Control::Control*> tab2Controls;

void update() {
  while (tab1Controls.size() > NUM_GLOBAL_CONTROLS) {
    tab1Controls.pop_back();
  }
  if (rvl::getDeviceMode() == rvl::DeviceMode::Controller) {
    tab1Controls.push_back(presetControl);
    for (auto& control : presets[preset]->controls) {
      tab1Controls.push_back(control);
    }
  } else {
    tab1Controls.push_back(remoteBrightnessControl);
  }
  rvl::emit(Codes::EventType::UIStateChange);
}

void getWiFiSSIDValue(char* buffer) {
#ifdef DEFAULT_WIFI_SSID
  snprintf(buffer, strlen(buffer), "%s", DEFAULT_WIFI_SSID);
#else
  snprintf(buffer, strlen(buffer), "N/A");
#endif
}
Control::LabelControl* wifiSSIDControl;

void getAddressValue(char* buffer) {
  if (rvl::isNetworkConnected()) {
    snprintf(buffer, strlen(buffer), "%d", rvl::getDeviceId());
  } else {
    snprintf(buffer, strlen(buffer), "N/A");
  }
}
Control::LabelControl* addressControl;

void getClockValue(char* buffer) {
  snprintf(buffer, strlen(buffer), "%d", rvl::getAnimationClock());
}
Control::LabelControl* clockControl;

void init() {
  resetScreenTimeout();
  preset = Settings::getSetting("ui-preset", DEFAULT_PRESET);
  brightnessControl = new Control::RangeControl("Brightness", 0, 16,
      getBrightnessValue(), updateBrightnessValue, getBrightnessValue);

  channelControl = new Control::ListControl("Channel",
      {"0", "1", "2", "3", "4", "5", "6", "7"}, rvl::getChannel(),
      updateChannelValue);

  modeControl = new Control::ListControl("Mode", {"Controller", "Receiver"},
      rvl::getDeviceMode() == rvl::DeviceMode::Controller ? 0 : 1,
      updateModeValue);

  presetControl = new Control::ListControl("Preset",
      {"Rainbow", "Pulse", "Wave", "Shift", "Color Cycle", "Solid"}, preset,
      updatePresetValue);

  remoteBrightnessControl =
      new Control::ListControl("Remote Brightness", {"Disabled", "Enabled"},
          rvl::getRemoteBrightnessState(), updateRemoteBrightnessValue);

  tab1Controls.push_back(brightnessControl);
  tab1Controls.push_back(channelControl);
  tab1Controls.push_back(modeControl);

  clockControl = new Control::LabelControl("Clock", getClockValue);
  addressControl = new Control::LabelControl("Node Address", getAddressValue);
  wifiSSIDControl = new Control::LabelControl("WiFi SSID", getWiFiSSIDValue);

  tab2Controls.push_back(clockControl);
  tab2Controls.push_back(addressControl);
  tab2Controls.push_back(wifiSSIDControl);

  presets.push_back(new Rainbow::Rainbow());
  presets.push_back(new Pulse::Pulse());
  presets.push_back(new Wave::Wave());
  presets.push_back(new Shift::Shift());
  presets.push_back(new ColorCycle::ColorCycle());
  presets.push_back(new Solid::Solid());

  rvl::on(EVENT_CONNECTION_STATE_CHANGED, update);
  rvl::on(Codes::EventType::AnimationChange, update);
  rvl::on(EVENT_DEVICE_MODE_UPDATED, update);
  rvl::on(EVENT_BRIGHTNESS_UPDATED, update);
  rvl::on(Codes::EventType::TimeChange, update);
  update();
  tab1Controls.reserve(10);
  tab2Controls.reserve(10);
  presets[preset]->updateWave();
}

void nextControl() {
  if (currentTab == 0) {
    if (currentTab1Control < tab1Controls.size() - 1) {
      currentTab1Control++;
      rvl::debug("Setting Tab 1 control to %d", currentTab1Control);
      rvl::emit(Codes::EventType::UIStateChange);
    }
  } else {
    if (currentTab2Control < tab2Controls.size() - 1) {
      currentTab2Control++;
      rvl::debug("Setting Tab 2 control to %d", currentTab2Control);
      rvl::emit(Codes::EventType::UIStateChange);
    }
  }
}

void previousControl() {
  if (currentTab == 0) {
    if (currentTab1Control > 0) {
      currentTab1Control--;
      rvl::debug("Setting Tab 1 control to %d", currentTab1Control);
      rvl::emit(Codes::EventType::UIStateChange);
    }
  } else {
    if (currentTab2Control > 0) {
      currentTab2Control--;
      rvl::debug("Setting Tab 2 control to %d", currentTab2Control);
      rvl::emit(Codes::EventType::UIStateChange);
    }
  }
}

void controlIncrease() {
  if (currentTab == 0) {
    tab1Controls[currentTab1Control]->increaseValue();
    rvl::emit(Codes::EventType::UIStateChange);
  } else {
    tab2Controls[currentTab2Control]->increaseValue();
    rvl::emit(Codes::EventType::UIStateChange);
  }
}

void controlDecrease() {
  if (currentTab == 0) {
    tab1Controls[currentTab1Control]->decreaseValue();
    rvl::emit(Codes::EventType::UIStateChange);
  } else {
    tab2Controls[currentTab2Control]->decreaseValue();
    rvl::emit(Codes::EventType::UIStateChange);
  }
}

void nextTab() {
  if (currentTab == 0) {
    currentTab = 1;
  } else {
    currentTab = 0;
  }
  rvl::emit(Codes::EventType::UIStateChange);
}

bool isCurrentControlRange() {
  if (currentTab == 0) {
    return tab1Controls[currentTab1Control]->type ==
        Control::ControlType::Range;
  } else {
    return tab2Controls[currentTab2Control]->type ==
        Control::ControlType::Range;
  }
}

void resetScreenTimeout() {
  screenTimeout = rvl::getAnimationClock() + SCREEN_OFF_TIME;
}

bool isScreenActive() {
#ifdef SCREEN_SAVER_ENABLED
  return rvl::getAnimationClock() < screenTimeout;
#else
  return true;
#endif
}

} // namespace UIState

#endif // HAS_UI
