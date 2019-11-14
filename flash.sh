#!/bin/sh
# Copyright (c) 2017 Bryan Hughes <bryan@nebri.us>

# This file is part of Raver Lights.

# Raver Lights is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Raver Lights is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Raver Lights.  If not, see <http://www.gnu.org/licenses/>.

target=$1
shift;

if [ $# -eq 0 ]
then
  echo "Compiling $target target\n"
else
  echo "Flashing $target target to ports $@\n"
fi

serialDevicePrefix=/dev/ttyS

./lint.sh && platformio run -e $target

if [ $# -eq 0 ]
then
  for port in "$@"
  do
    echo "\nProgramming device at $serialDevicePrefix$port"
    esptool.py -p $serialDevicePrefix$port -b 921600 write_flash 0x0 .pio/build/$firmware/firmware.bin
  done
fi
