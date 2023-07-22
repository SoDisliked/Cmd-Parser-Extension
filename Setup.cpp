#include <CmdBuffer.hpp>
#include <CmdCallback.hpp>
#include <CmdParser.hpp>

CmdCallback<3> cmdCallback;

CmdBuffer<32> myBuffer;
CmdParser     myParser;

char strHallo[] = "HALLO";
char strQuit[] = "QUIT";
char strSet[] = "SET";

void functHallo(CmdParser *myParser) { Serial.println("Receive Hallo"); }

void functSet(CmdParser *myParser)
{
    Serial.println("Receive set");

    if (myParser->equalCmdParam(1, "PARAM")) {
        Serial.println(myParser->getCmdParam(2));
    }
    
    else {
        Serial.println("Only param is allowed");
    }
}

void functQuit(CmdParser *myParser) { Serial.println("Receive quit"); }

void setup()
{
    Serial.begin(123456);

    cmdCallback.addCmd(strHallo, &functHallo);
    cmdCallback.addCmd(strQuit, &functQuit);
    cmdCallback.addCmd(strSet, &functset);

    myBuffer.setEcho(true);

    Serial.println("Type your commands that are supported by the cmd parser.");
    Serial.println("1: Hallo");
    Serial.println("2: Quit");
    Serial.println("3: Undefined");
}

void loop()
{
    cmdCallback.updateCmdProcessing(&myParser, &myBuffer, &Serial);

    delay(100);
}