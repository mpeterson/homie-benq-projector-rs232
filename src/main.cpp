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

#include <vector>

#include <Homie.h>

#include "main.hpp"
#include "SerialManager.hpp"

HomieNode projectorNode("benq", "projector");
SerialManager serialManager(Serial);

bool setProperty(const String &property, const String &value, const String &serialCommand, std::vector<String> &validOptions)
{
    bool validated = false;
    for (String s : validOptions)
    {
        if (s == value)
        {
            validated = true;
            break;
        }
    }

    if (!validated)
    {
        return false;
    }

    mqttLog("Setting property=" + property + " value=" + value);
    if (!serialManager.sendCommand(serialCommand, value))
    {
        return false;
    }

    
    projectorNode.setProperty(property).send(value);

    return true;
}

void getSource(const String &value)
{
    projectorNode.setProperty(MQTT_SOURCE).send(value);
}

bool setSource(const HomieRange &range, const String &value)
{
    std::vector<String> validOptions{VALUE_HDMI1, VALUE_HDMI2};
    return setProperty(MQTT_SOURCE, value, CMD_SOURCE, validOptions);
}

bool setVolume(const HomieRange &range, const String &value)
{
    std::vector<String> validOptions{VALUE_VOL_UP, VALUE_VOL_DOWN};
    return setProperty(MQTT_VOLUME, value, CMD_VOLUME, validOptions);
}

void getPower(const String &value)
{
    mqttLog("Getting property=power value=" + value);
    projectorNode.setProperty(MQTT_POWER).send(value);
}

bool setPower(const HomieRange &range, const String &value)
{
    std::vector<String> validOptions{VALUE_ON, VALUE_OFF};
    return setProperty(MQTT_POWER, value, CMD_POWER, validOptions);
}

void getMute(const String &value)
{
    projectorNode.setProperty(MQTT_MUTE).send(value);
}

bool setMute(const HomieRange &range, const String &value)
{
    std::vector<String> validOptions{VALUE_ON, VALUE_OFF};
    return setProperty(MQTT_MUTE, value, CMD_MUTE, validOptions);
}

bool setSerialRead(const HomieRange &range, const String &value)
{
    serialManager.sendCommand(value, "?");

    return true;
}

void mqttLog(const String &value)
{
    projectorNode.setProperty(MQTT_DEBUG).send(value);
}

void setup()
{
    Homie.disableLogging();

    Homie_setFirmware(FW_NAME, FW_VERSION);
    Homie.setup();

    projectorNode.advertise(MQTT_DEBUG);
    projectorNode.setProperty(MQTT_DEBUG).setRetained(false);
    projectorNode.advertise(MQTT_POWER).settable(setPower);
    projectorNode.advertise(MQTT_SOURCE).settable(setSource);
    projectorNode.advertise(MQTT_VOLUME).settable(setVolume);
    projectorNode.advertise(MQTT_MUTE).settable(setMute);

    serialManager.setLogger(mqttLog);

    serialManager.setup();

    serialManager.monitor(CMD_POWER, getPower);
    serialManager.monitor(CMD_SOURCE, getSource);
    serialManager.monitor(CMD_MUTE, getMute);
    
    mqttLog("Node initialization complete");
}

void loop()
{
    Homie.loop();
    serialManager.loop();
}
