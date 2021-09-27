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

#ifndef INCLUDE_SERIALMANAGER_HPP_
#define INCLUDE_SERIALMANAGER_HPP_

#include <functional>
#include <forward_list>
#include <map>

#include "HardwareSerial.h"
#include "WString.h"

#define SERIAL_BUFFER_SIZE 256
#define SERIAL_BAUD  9600
#define SERIAL_HEAD  "\r*"
#define SERIAL_TOKEN "="
#define SERIAL_TRAIL "#\r"

#define SERIAL_CMD_UNSUPPORTED "Unsupported item"
#define SERIAL_CMD_FAILED "Block item"
#define SERIAL_CMD_ILLEGAL "Illegal format"

#define CMD_POWER "POW"

#define CMD_SOURCE "SOUR"

#define CMD_VOLUME "VOL"
#define CMD_MUTE "MUTE"

#define VALUE_ON "ON"
#define VALUE_OFF "OFF"

#define VALUE_VOL_UP "+"
#define VALUE_VOL_DOWN "-"

#define VALUE_HDMI1 "HDMI"
#define VALUE_HDMI2 "HDMI2"

# define DEFAULT_POLLING_DELAY 20000

typedef std::function<void(const String& value)> SerialInputHandler;
typedef std::map<String, SerialInputHandler> MonitoredMap;

void dummyLogger(const String &value);

class SerialManager {
    public:
        unsigned long pollingDelay = DEFAULT_POLLING_DELAY;

        SerialManager(HardwareSerial &serial);

        void setup();
        void loop();

        void setLogger(const SerialInputHandler &handler);

        void monitor(const String &command, const SerialInputHandler &handler);

        bool sendCommand(const String &command, const String &value);
        String readCommand();

    private:
        HardwareSerial &_serial;
        
        long _lastPoll;

        std::forward_list<String> _sentCommands;
        MonitoredMap _monitoredCommands;
        SerialInputHandler _logger;

        std::pair<String, String> _parseInput(const String &input);
        void _poll();
        void _recv();
};

#endif