/*
 * @brief: Command interace for the arduino Operating System interace
 * Valentijn Hulst, 2019
 */

#include "cmdInterface.h"

char inputArray[12];
char ch;
bool incomingData = false;

void setup()
{
    Serial.begin(9600);
}


void loop()
{
    scanBuffer();
    
}


int scanBuffer()
{
    if(Serial.available() > 0)
    {
        inputArray = Serial.read();
        incomingData = true;

    }

    if(ch != '\n')
    {
        incomingData = false;
        continue;
    }
     strcat(inputArray, '\0');
}

void printBuffer()
{
    if(incomingData == true)
    {
        Serial.print("Your input is: ");
        Serial.println(inputArray);
        incomingData = false;
    }
}


