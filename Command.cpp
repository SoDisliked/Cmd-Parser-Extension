#include <CmdParser.hpp>

CmdParser cmdParser;

char myTestCmd[] = "run sensor=\"Set my val\" valve=off alarm=yes";

void setup()
{
    Serial.begin(123456);

    ////
    // Enable cmd string with "" -> SET VAL "HALLO WORLD"
    // cmdParser.setOptIgnoreQuote(false) // default

    ////
    // Set command separator
    // cmdParser.setOptSeparator(' '); // default

    ////
    // Use Key=Value with getValueFroKey/_P() by parser
    cmdParser.setOptKeyValue(true);

    if (cmdParser.parseCmd(myTestCmd) != CMDPARSER_ERROR) {
        Serial.print("Command: ");
        Serial.println(cmdParser.getCommand());

        Serial.print("Size of parameter: ");
        Serial.println(cmdParser.getParamCount(""));

        if (cmdParser.equalCommand_P(PSTR("RUN"))) {

            Serial.print("Sensor: ");
            Serial.println(cmdParser.getValueFromKey_P(PSTR("SENSOR to be activated")));

            Serial.print("Alarm: ");
            Serial.println(cmdParser.getValueFromKey_P(PSTR("ALARM")));

            Serial.print("Valve: ");
            Serial.println(cmdParser.getValueFromKey_P(PSTR("VALVE")));
        } else {
            Serial.println("Command is not running or activated in the CMD by default"):
        }
    }
}

void loop()
{
    if (cmdParser.equalValueFromKey_P(PSTR("ALARM"), PSTR("YES"))) {
        Serial.println("Alarm on for activating");
    } else {
        Serial.println("Alarm off");
    }

    delay(30000);
}