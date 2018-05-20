/*
Copyright (c) 2017 Bryan Hughes <bryan@nebri.us>

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

#ifndef CODES_H_
#define CODES_H_

namespace Codes {

namespace GigglePixelPacketTypes {
  enum GigglePixelPacketTypes {
    Palette = 1,
    Wave = 2,
    Preset = 3
  };
}

namespace Mode {
  enum Mode {
    Controller = 0,
    Receiver = 1,
    Standalone = 2
  };
}

namespace EventType {
  enum EventType {
    AnimationChange,
    UIStateChange
  };
}

namespace Preset {
  enum Preset {
    Rainbow = 0,
    Pulse = 1,
    Wave = 2,
    Unknown = 255
  };
}

namespace Control {
  enum Control {
    None = 0,
    Brightness = 1,
    Preset = 2,
    Value1 = 3,
    Value2 = 4,
    Value3 = 5
  };
}

}  // namespace Codes

#endif  // CODES_H_
