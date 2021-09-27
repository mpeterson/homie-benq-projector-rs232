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

#include <vector>

#define FW_NAME "homie-benq-projector-rs232"
#define FW_VERSION "0.4.1"

#define MQTT_DEBUG "debug"

#define MQTT_POWER "power"
#define MQTT_SOURCE "source"
#define MQTT_VOLUME "volume"
#define MQTT_MUTE "mute"

bool setProperty(const String &property, const String &value, const String &serialCommand, std::vector<String> &validOptions);

bool setVolume(const HomieRange &range, const String &value);

void getSource(const String &value);
bool setSource(const HomieRange &range, const String &value);

void getPower(const String &value);
bool setPower(const HomieRange &range, const String &value);

void getMute(const String &value);
bool setMute(const HomieRange &range, const String &value);

bool setSerialRead(const HomieRange &range, const String &value);

void mqttLog(const String &value);

#endif // INCLUDE_MAIN_HPP_
