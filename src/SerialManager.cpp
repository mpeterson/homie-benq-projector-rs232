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

#include "Arduino.h"
#include "WString.h"

#include "SerialManager.hpp"

void dummyLogger(const String &value) {};

SerialManager::SerialManager(HardwareSerial &serial)
    : _serial(serial),
      _sentCommands(),
      _monitoredCommands(),
      _logger(dummyLogger)
{
    _lastPoll = millis();
};

void SerialManager::setup()
{
    _serial.begin(SERIAL_BAUD);
    _serial.swap();
    _serial.setTimeout(100);
}

void SerialManager::setLogger(const SerialInputHandler &handler) {
    _logger = handler;
}

void SerialManager::monitor(const String &command, const SerialInputHandler &handler)
{
    _monitoredCommands.insert(std::make_pair(command, handler));
}

std::pair<String, String> SerialManager::_parseInput(const String &input)
{
    int idx = input.indexOf(SERIAL_TOKEN);
    String command = input.substring(0, idx);
    String value = input.substring(idx + 1);
    return std::make_pair(command, value);
}

void SerialManager::_poll()
{
    for (auto &item : _monitoredCommands)
    {
        sendCommand(item.first, "?");
    }
}

void SerialManager::_recv()
{
    String input = readCommand();

    if (input.length() == 0)
    {
        return;
    }

    _logger("Reading on serial input=" + input);

    if (input == SERIAL_CMD_FAILED || input == SERIAL_CMD_UNSUPPORTED || input == SERIAL_CMD_ILLEGAL)
    {
        return;
    }

    auto command = _parseInput(input);

    auto iterator = _monitoredCommands.find(command.first);
    if (iterator == _monitoredCommands.end())
    {
        return;
    }

    SerialInputHandler handler = iterator->second;
    handler(command.second);
}

void SerialManager::loop()
{
    if (millis() - _lastPoll > pollingDelay)
    {
        _poll();
        _lastPoll = millis();
    }
    _recv();
};

bool SerialManager::sendCommand(const String &command, const String &value)
{
    String serialCommand = (SERIAL_HEAD + command + SERIAL_TOKEN + value + SERIAL_TRAIL);
    _logger("Sending on serial command=*" + command + SERIAL_TOKEN + value + "#");
    _serial.print(serialCommand);
    _sentCommands.push_front(command + SERIAL_TOKEN + value);
    // Make sure we wait the command is processed. The flush is not enough since the projector 
    // needs to process the request before sending the next one.
    _serial.flush();
    delay(50);

    return true;
}

String SerialManager::readCommand()
{
    String command = "";

    // 5 == SERIAL_HEAD + SERIAL_TOKEN + SERIAL_TRAIL
    if (_serial.available() < 6)
    {
        return "";
    }

    command = _serial.readStringUntil('\r');

    int start = command.indexOf("*");
    if (start == -1)
    {
        return "";
    }
    start += 1;

    int end = command.indexOf("#", start);
    if (end == -1)
    {
        return "";
    }

    command = command.substring(start, end);

    // Discard if it is a command we sent in the past
    auto oit = _sentCommands.before_begin(), it = std::next(oit);
    while (it != _sentCommands.end())
    {
        if (command == *it)
        {
            _sentCommands.erase_after(oit);
            return "";
        }
        oit = it++;
    }

    return command;
}