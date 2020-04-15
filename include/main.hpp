/*
BenQ Projector MQTT <-> RS232 controller based on the Homie MQTT convention
Copyright (C) 2018  Michel Peterson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_MAIN_HPP_
#define INCLUDE_MAIN_HPP_

#define FW_NAME      "homie-benq-projector-rs232"
#define FW_VERSION   "0.2.1"

#define SERIAL_BAUD  115200
#define SERIAL_HEAD  "\r*"
#define SERIAL_TOKEN "="
#define SERIAL_TRAIL "#\r"


void sendCommand(const String& command, const String& value);

bool toggleHandler(const String& value, const String& cmd_key, const String& opt_1, const String& opt_2, const String& node_property);

bool volumeHandler(const HomieRange& range, const String& value);

bool sourceHandler(const HomieRange& range, const String& value);

bool powerHandler(const HomieRange& range, const String& value);

#endif  // INCLUDE_MAIN_HPP_
