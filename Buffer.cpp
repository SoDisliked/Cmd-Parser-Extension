#include <CmdBuffer.hpp>
#include <CmdCallback.hpp>
#include <CmdParser.hpp>

CmdCallback_P<3> cmdCallback;

void setup()
{
    Serial.begin(123456);

    cmdCallback.addCmd(PSTR("HALLO"), &functHallo);
    cmdCallback.addCmd(PSTR("QUIT"), &functQuit);
    cmdCallback.addCmd(PSTR("SET"), &functSet);
}

void loop()
{
    CmdBuffer<32> myBuffer;
    CmdParser     myParser;

    Serial.println("Type the supported commands of the defined parser.");
    Serial.println("1: Hallo");
    Serial.println("2: Quit");
    Serial.println("3: not supported");

    cmdCallback.loopCmdProcessing(&myParser, &myBuffer, &Serial);
}

void functHallo(CmdParser *myParser) { Serial.println("Receive hallo"); }

void functSet(CmdParser *myParser)
{
    Serial.println("Receive set");

    // Alarm
    if (myParser->equalCmdParam_P(1, PSTR("ALARM"))) {
        Serial.println(myParser->getCmdParam(2));
    }
    else {
        Serial.println("Only alarm is allowed during the command's execution");
    }
}

void functQuit(CmdParser *myParser) { Serial.println("Receive quit"); }