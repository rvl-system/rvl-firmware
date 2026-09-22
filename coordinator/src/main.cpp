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

#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
#include <rvl/config.hpp>
#include <string.h>

#define WIFI_CHANNEL 3

// A set is NUM_OBSERVATIONS_IN_SET broadcasts spaced REFERENCE_SPACING apart,
// with SET_PERIOD from the start of one set to the start of the next
#define SET_PERIOD 2300
#define REFERENCE_SPACING 100

// One socket per protocol: rvlaUdp broadcasts clock references to the fleet,
// rvliUdp answers coordinator requests. Separate so neither drains the other
WiFiUDP rvlaUdp;
WiFiUDP rvliUdp;

uint16_t id = 0;
uint8_t deviceId = 0;

// Reference broadcasts are scheduled rather than delayed so loop() always
// returns promptly. Anything that blocks here stops the coordinator answering
// the requests it also serves
uint32_t setStartTime = 0;
uint32_t nextReferenceTime = 0;
uint8_t referencesSentInSet = NUM_OBSERVATIONS_IN_SET;

void write8(WiFiUDP& udp, uint8_t data) {
  udp.write(data);
}

void write16(WiFiUDP& udp, uint16_t data) {
  udp.write(data >> 8);
  udp.write(data & 0xFF);
}

void write(WiFiUDP& udp, uint8_t* data, uint16_t length) {
  size_t written = udp.write(data, length);
  if (written != length) {
    Serial.println("Error sending buffer");
  }
}

void handleRvliPacket() {
  if (rvliUdp.parsePacket() == 0) {
    return;
  }

  uint8_t header[8];
  int read = rvliUdp.read(header, sizeof(header));
  IPAddress requester = rvliUdp.remoteIP();
  uint16_t requesterPort = rvliUdp.remotePort();

  if (read < static_cast<int>(sizeof(header)) ||
      memcmp(header, rvl::rvliSignature, 4) != 0)
  {
    rvliUdp.clear();
    return;
  }
  if (header[4] != PROTOCOL_VERSION) {
    Serial.printf("Ignoring RVLI version %d\n", header[4]);
    rvliUdp.clear();
    return;
  }

  // header[5] is the source, which the coordinator has no use for
  switch (header[6]) {
  case RVLI_PACKET_TYPE_ID_ASSIGNMENT: {
    uint8_t subType = rvliUdp.read();
    rvliUdp.clear();
    if (subType != ID_REQUEST_TYPE) {
      return; // A reply, which we send rather than receive
    }
    // The requester's own address is what makes the ID unique without an
    // allocator here: the DHCP server already guarantees no two stations share
    // one, and it is the only party that can. That derivation is private to the
    // coordinator, so a future non-IP transport can hand out something else
    uint8_t assignedId = requester[3];
    if (assignedId >= NUM_DEVICE_IDS) {
      // The pool is exhausted, or the AP moved onto a subnet whose addresses
      // can't be device IDs. Say nothing: silence is wire-identical to being
      // down, which the requester already retries against
      Serial.printf("Cannot assign ID %d, out of range\n", assignedId);
      return;
    }
    rvliUdp.beginPacket(requester, requesterPort);
    write(rvliUdp, rvl::rvliSignature, 4);
    write8(rvliUdp, PROTOCOL_VERSION);
    write8(rvliUdp, deviceId);
    write8(rvliUdp, RVLI_PACKET_TYPE_ID_ASSIGNMENT);
    write8(rvliUdp, 0); // Reserved
    write8(rvliUdp, ID_REPLY_TYPE);
    write8(rvliUdp, assignedId);
    rvliUdp.endPacket();
    Serial.printf(
        "Assigned ID %d to %s\n", assignedId, requester.toString().c_str());
    break;
  }
  default:
    Serial.printf("Ignoring unknown RVLI packet type %d\n", header[6]);
    rvliUdp.clear();
    break;
  }
}

void sendReferenceBroadcast(bool isStartOfSet) {
  IPAddress ip(255, 255, 255, 255);
  rvlaUdp.beginPacket(ip, RVLA_PORT);

  write(rvlaUdp, rvl::rvlaSignature, 4);
  write8(rvlaUdp, PROTOCOL_VERSION);
  write8(rvlaUdp, 255); // Destination: broadcast
  write8(rvlaUdp, deviceId);
  write8(rvlaUdp, PACKET_TYPE_CLOCK_SYNC);
  write8(rvlaUdp, 0); // Channel: clock sync is channel independent
  write8(rvlaUdp, 0); // Reserved

  write8(rvlaUdp, 1); // Clock sync subpacket type: reference broadcast
  write16(rvlaUdp, id);
  write8(rvlaUdp, 0); // Reserved
  write8(rvlaUdp, isStartOfSet ? 1 : 0);
  write8(rvlaUdp, 0); // Reserved

  rvlaUdp.endPacket();
}

void setup() {
  Serial.begin(115200);

  Serial.println("Setting AP (Access Point)...");
  WiFi.softAP(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSPHRASE, WIFI_CHANNEL);

  wifi_config_t conf;
  esp_wifi_get_config(WIFI_IF_AP, &conf);
  conf.ap.dtim_period = 1;
  conf.ap.max_connection = 15;
  esp_wifi_set_config(WIFI_IF_AP, &conf);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);

  // Boards drop any packet whose source is >= 240, so an AP address ending
  // there would silently kill clock sync for the whole fleet.
  //
  // This ID must also never be one a board can be assigned, or that board would
  // drop every reference as its own. That holds because the DHCP server never
  // leases its own address, which is only true while the coordinator is the AP
  deviceId = ip[3];
  if (deviceId >= NUM_DEVICE_IDS) {
    Serial.println("ERROR: AP address ends >= 240, boards will ignore us");
  }

  rvlaUdp.begin(RVLA_PORT);
  rvliUdp.begin(RVLI_PORT);
}

void loop() {
  // Always first, and never behind an early return: a board with no ID can't do
  // anything until this is answered, and it retries on a ~1s timer
  handleRvliPacket();

  uint32_t now = millis();
  if (static_cast<int32_t>(now - nextReferenceTime) < 0) {
    return;
  }

  if (referencesSentInSet >= NUM_OBSERVATIONS_IN_SET) {
    // Receivers map each broadcast to an observation slot via
    // id % NUM_OBSERVATIONS_IN_SET, so every broadcast in a set needs its own
    // consecutive id. Reset before the uint16 wrap so a set never spans it,
    // which would map two broadcasts to one slot
    if (id > UINT16_MAX - NUM_OBSERVATIONS_IN_SET) {
      id = 0;
    }
    referencesSentInSet = 0;
    setStartTime = now;
  }

  sendReferenceBroadcast(referencesSentInSet == 0);
  id++;
  referencesSentInSet++;

  // Sets are phase locked to setStartTime rather than chained off the last
  // send, so a late loop iteration doesn't push the whole schedule back
  nextReferenceTime = referencesSentInSet < NUM_OBSERVATIONS_IN_SET
      ? now + REFERENCE_SPACING
      : setStartTime + SET_PERIOD;
}
