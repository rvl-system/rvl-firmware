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

#ifdef HAS_UI

#include "./ui/ui_state.hpp"
#include "./presets/color_cycle.hpp"
#include "./presets/preset_control_set.hpp"
#include "./presets/pulse.hpp"
#include "./presets/rainbow.hpp"
#include "./presets/shift.hpp"
#include "./presets/solid.hpp"
#include "./presets/wave.hpp"
#include "./settings.hpp"
#include <Arduino.h>
#include <rvl.hpp>

namespace UIState {

uint8_t currentTab1Control = 0;
uint8_t currentTab2Control = 0;
uint8_t preset;
uint8_t currentTab = 0;
uint32_t lastInteractionTime = 0;

std::vector<PresetControlSet*> presets;

void update();

uint8_t getBrightnessValue() {
  return 16 * (rvl::getBrightness() - MIN_BRIGHTNESS) /
      (MAX_BRIGHTNESS - MIN_BRIGHTNESS);
}
void updateBrightnessValue(uint8_t newValue) {
  uint16_t adjustedBrightness =
      (newValue * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / 16) + MIN_BRIGHTNESS;
  rvl::setBrightness(adjustedBrightness);
  rvl::info("Changing brightness to %d", adjustedBrightness);
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
      presets[preset]->updateParameters();
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
    presets[preset]->updateParameters();
    update();
  }
}
Control::ListControl* presetControl;

// Every list tab 1 can show, built in init() and never changed after, since the
// foreground task renders whichever one it's handed while the background task
// switches between them
std::vector<Control::Control*> receiverTab1Controls;
std::vector<std::vector<Control::Control*>> controllerTab1Controls;
std::atomic<std::vector<Control::Control*>*> tab1Controls{
    &receiverTab1Controls};

std::vector<Control::Control*> tab2Controls;

void update() {
  if (rvl::getDeviceMode() == rvl::DeviceMode::Controller) {
    tab1Controls = &controllerTab1Controls[preset];
  } else {
    tab1Controls = &receiverTab1Controls;
  }
}

void getWiFiSSIDValue(char* buffer) {
#ifdef DEFAULT_WIFI_SSID
  snprintf(buffer, strlen(buffer), "%s", DEFAULT_WIFI_SSID);
#else
  snprintf(buffer, strlen(buffer), "N/A");
#endif
}
Control::LabelControl* wifiSSIDControl;

void getDeviceIdValue(char* buffer) {
  if (rvl::isConnected()) {
    snprintf(buffer, strlen(buffer), "%d", rvl::getDeviceId());
  } else {
    snprintf(buffer, strlen(buffer), "Pending");
  }
}
Control::LabelControl* deviceIdControl;

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

  receiverTab1Controls = {brightnessControl, channelControl, modeControl};

  clockControl = new Control::LabelControl("Clock", getClockValue);
  deviceIdControl = new Control::LabelControl("Device ID", getDeviceIdValue);
  wifiSSIDControl = new Control::LabelControl("WiFi SSID", getWiFiSSIDValue);

  tab2Controls.push_back(clockControl);
  tab2Controls.push_back(deviceIdControl);
  tab2Controls.push_back(wifiSSIDControl);

  presets.push_back(new Rainbow::Rainbow());
  presets.push_back(new Pulse::Pulse());
  presets.push_back(new Wave::Wave());
  presets.push_back(new Shift::Shift());
  presets.push_back(new ColorCycle::ColorCycle());
  presets.push_back(new Solid::Solid());

  for (auto* presetControlSet : presets) {
    auto controls = receiverTab1Controls;
    controls.push_back(presetControl);
    controls.insert(controls.end(), presetControlSet->controls.begin(),
        presetControlSet->controls.end());
    controllerTab1Controls.push_back(controls);
  }

  rvl::on(EVENT_DEVICE_MODE_UPDATED, update);
  update();
  presets[preset]->updateParameters();
}

void nextControl() {
  if (currentTab == 0) {
    if (currentTab1Control < tab1Controls.load()->size() - 1) {
      currentTab1Control++;
      rvl::debug("Setting Tab 1 control to %d", currentTab1Control);
    }
  } else {
    if (currentTab2Control < tab2Controls.size() - 1) {
      currentTab2Control++;
      rvl::debug("Setting Tab 2 control to %d", currentTab2Control);
    }
  }
}

void previousControl() {
  if (currentTab == 0) {
    if (currentTab1Control > 0) {
      currentTab1Control--;
      rvl::debug("Setting Tab 1 control to %d", currentTab1Control);
    }
  } else {
    if (currentTab2Control > 0) {
      currentTab2Control--;
      rvl::debug("Setting Tab 2 control to %d", currentTab2Control);
    }
  }
}

void controlIncrease() {
  if (currentTab == 0) {
    (*tab1Controls)[currentTab1Control]->increaseValue();
  } else {
    tab2Controls[currentTab2Control]->increaseValue();
  }
}

void controlDecrease() {
  if (currentTab == 0) {
    (*tab1Controls)[currentTab1Control]->decreaseValue();
  } else {
    tab2Controls[currentTab2Control]->decreaseValue();
  }
}

void nextTab() {
  if (currentTab == 0) {
    currentTab = 1;
  } else {
    currentTab = 0;
  }
}

bool isCurrentControlRange() {
  if (currentTab == 0) {
    return (*tab1Controls)[currentTab1Control]->type ==
        Control::ControlType::Range;
  } else {
    return tab2Controls[currentTab2Control]->type ==
        Control::ControlType::Range;
  }
}

void resetScreenTimeout() {
  lastInteractionTime = millis();
}

bool isScreenActive() {
#ifdef SCREEN_SAVER_ENABLED
  return millis() - lastInteractionTime < SCREEN_OFF_TIME;
#else
  return true;
#endif
}

} // namespace UIState

#endif // HAS_UI
