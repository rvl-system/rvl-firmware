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

#ifndef MESSAGING_STACK_TRANSPORT_H_
#define MESSAGING_STACK_TRANSPORT_H_

#include <Arduino.h>

class TransportInterface {
 public:
  virtual void beginWrite() = 0;
  virtual void write8(uint8 data) = 0;
  virtual void write16(uint16 data) = 0;
  virtual void write32(uint32 data) = 0;
  virtual void write(byte* data, uint16 length) = 0;
  virtual void endWrite() = 0;

  virtual uint8 read8() = 0;
  virtual uint16 read16() = 0;
  virtual uint32 read32() = 0;
  virtual void read(uint8* buffer, int length) = 0;
};

#endif  // MESSAGING_STACK_TRANSPORT_H_
