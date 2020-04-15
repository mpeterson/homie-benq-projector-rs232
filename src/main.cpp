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

#include <Homie.h>

#include "main.hpp"


HomieNode projectorNode("benq", "projector");

void sendCommand(const String& command, const String& value) {
    Serial.print(SERIAL_HEAD);
    Serial.print(command);
    Serial.print(SERIAL_TOKEN);
    Serial.print(value);
    Serial.print(SERIAL_TRAIL);
}

bool toggleHandler(const String& value, const String& cmd_key, const String& opt_1, const String& opt_2, const String& node_property)
{
    if (value != opt_1 && value != opt_2) {
        return false;
    }

    sendCommand(cmd_key, value);
    projectorNode.setProperty(node_property).send(value);

    return true;
}


bool sourceHandler(const HomieRange& range, const String& value) {
    return toggleHandler(value, "sour", "hdmi1", "hdmi2", "source");
}

bool volumeHandler(const HomieRange& range, const String& value){
    // TODO: note that property volume can be also read with <CR>*vol=?#<CR> and thus publish the actual volume value instead of +/-
    return toggleHandler(value, "vol", "+", "-", "volume");
}

bool powerHandler(const HomieRange& range, const String& value) {
    return toggleHandler(value, "pow", "on", "off", "power");
}

void setup() {
    Homie.disableLogging();
    Serial.begin(SERIAL_BAUD);
    Serial.swap();

    Homie_setFirmware(FW_NAME, FW_VERSION);
    Homie.setup();

    projectorNode.advertise("power").settable(powerHandler);
    projectorNode.advertise("source").settable(sourceHandler);
    projectorNode.advertise("volume").settable(volumeHandler);
}

void loop() {
    Homie.loop();
}
