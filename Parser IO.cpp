#include <CmdBuffer.hpp>

CmdBuffer<64> cmdBuffer;

void setup()
{
    Serial.begin(123456);
}

void loop()
{
    Serial.println("Start reading");

    if (cmdBuffer.readFromSerial(&Serial, 30000)) {
        Serial.println("Line has been read");
        Serial.println(cmdBuffer.getStringFromBuffer());
    } else {
        Serial.println("TIMEOUT");
    }
}