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

namespace EventTypes {
  enum EventTypes {
    AnimationChange,
    ScreenChange,
    InputChange
  };
}

namespace Preset {
  enum Preset {
    Fade = 0,
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

namespace IdleState {
  enum IdleState {
    Active = 0,
    ShallowIdle = 1,
    DeepIdle = 2
  };
}

}  // namespace Codes

#endif  // CODES_H_
