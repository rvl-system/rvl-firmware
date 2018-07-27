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

#ifndef UTIL_LOGGING_H_
#define UTIL_LOGGING_H_

#include <Arduino.h>

namespace Logging {

inline void init(int32 baudrate) {
  Serial.begin(baudrate);
}

// Base case versions

inline void log(const char *s) {
  Serial.print(s);
}

inline void error(const char *s) {
#ifdef LOG_ERROR_ENABLED
  Serial.print("[error]: ");
  Serial.println(s);
#endif
}

inline void info(const char *s) {
#ifdef LOG_INFO_ENABLED
  Serial.print("[info ]: ");
  Serial.println(s);
#endif
}

inline void debug(const char *s) {
#ifdef LOG_DEBUG_ENABLED
  Serial.print("[debug]: ");
  Serial.println(s);
#endif
}

// Variadic versions

template<typename T, typename ...Args>
inline void log(const char *s, T value, Args ...args) {
  while (*s) {
    if (*s == '%' && *(++s) != '%') {
      Serial.print(value);
      s++;
      log(s, args...);
      return;
    }
    Serial.print(*(s++));
  }
}

template<typename T, typename... Args>
inline void error(const char *s, T value, Args... args) {
#ifdef LOG_ERROR_ENABLED
  Serial.print("[error]: ");
  log(s, value, args...);
  Serial.println();
#endif
}

template<typename T, typename... Args>
inline void info(const char *s, T value, Args... args) {
#ifdef LOG_INFO_ENABLED
  Serial.print("[info ]: ");
  log(s, value, args...);
  Serial.println();
#endif
}

template<typename T, typename... Args>
inline void debug(const char *s, T value, Args... args) {
#ifdef LOG_DEBUG_ENABLED
  Serial.print("[debug]: ");
  log(s, value, args...);
  Serial.println();
#endif
}

}  // namespace Logging

#endif  // UTIL_LOGGING_H_
