#include "processManager.h"

processManager ProcessManagerObj;

void setup()
{
    Serial.begin(9600);
    ProcessManagerObj.executeProcess();
}


void loop()
{}