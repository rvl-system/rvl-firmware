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

#include "./controls/controls.hpp"
#include "./config.hpp"
#include <Arduino.h>
#include <rvl-wifi.hpp>

namespace Controls {

bool connectedLEDState = false;
uint32_t nextConnectedLEDFlashTime = 0;

struct Bucket {
  uint16_t start;
  uint16_t end;
};

std::vector<Bucket*> buckets;
uint8_t currentBucketIndex;
uint32_t lastConnectedTime = 0;

void updateBrightness(uint16_t input) {
#ifdef HAS_CONTROLS
  auto currentBucket = buckets[currentBucketIndex];
  if (input < currentBucket->start) {
    currentBucketIndex--;
  } else if (input >= currentBucket->end) {
    currentBucketIndex++;
  }

  auto newBrightness = currentBucketIndex * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) /
          NUM_BRIGHTNESS_BUCKETS +
      MIN_BRIGHTNESS;

  if (newBrightness != rvl::getBrightness()) {
    rvl::info("Brightness changed to %d", newBrightness);
    rvl::setBrightness(newBrightness);
  }
#endif
}

void updateChannel() {
#ifdef HAS_CONTROLS
  auto digit1 = digitalRead(CONTROL_DIGIT_1);
  auto digit2 = digitalRead(CONTROL_DIGIT_2);
  auto digit3 = digitalRead(CONTROL_DIGIT_3);
  auto channel = digit1 * 4 + digit2 * 2 + digit3;
  if (rvl::getChannel() != channel) {
    rvl::info("Channel changed to %d", channel);
    rvl::setChannel(channel);
  }
#endif
}

void init() {
#ifdef HAS_CONTROLS
  pinMode(CONTROL_DIGIT_1, INPUT);
  pinMode(CONTROL_DIGIT_2, INPUT);
  pinMode(CONTROL_DIGIT_3, INPUT);

  pinMode(BRIGHTNESS_PIN, ANALOG);

  // Initialize the buckets
  uint16_t i = 0;
  while (i < MAX_ADC_INPUT - BUCKET_WIDTH) {
    auto newBucket = new Bucket();
    newBucket->start = i;
    newBucket->end = i + BUCKET_WIDTH;
    buckets.push_back(newBucket);
    i += BUCKET_WIDTH / 2;
  }
  auto lastBucket = new Bucket();
  lastBucket->start = i;
  lastBucket->end = MAX_ADC_INPUT + 1;
  buckets.push_back(lastBucket);

  // Determine the initial bucket
  auto initialInput = analogRead(BRIGHTNESS_PIN);
  if (initialInput < buckets[1]->start) {
    currentBucketIndex = 0;
  } else {
    auto numBuckets = buckets.size();
    for (uint8_t i = 0; i < numBuckets; i++) {
      if (i == numBuckets - 1) {
        currentBucketIndex = i;
        break;
      }
      auto currentBucket = buckets[i];
      auto nextBucket = buckets[i];
      if (currentBucket->start <= initialInput &&
          currentBucket->end > initialInput &&
          nextBucket->start < initialInput && nextBucket->end > initialInput)
      {
        auto currentBucketDistance = abs((initialInput - currentBucket->start) -
            (currentBucket->end - initialInput));
        auto nextBucketDistance = abs((initialInput - nextBucket->start) -
            (nextBucket->end - initialInput));
        currentBucketIndex =
            currentBucketDistance < nextBucketDistance ? i : i + 1;
        break;
      }
    }
  }

  updateChannel();
  updateBrightness(initialInput);

  rvl::info("Controls initialized");
#endif
}

void loop() {
#ifdef HAS_CONTROLS
  updateChannel();
  updateBrightness(analogRead(BRIGHTNESS_PIN));
#endif
}

} // namespace Controls
