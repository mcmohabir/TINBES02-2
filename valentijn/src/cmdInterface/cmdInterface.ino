/*
   @brief: Command interace for the arduino Operating System interace
   Valentijn Hulst, 2019
*/

#include "cmdInterface.h"

const int arrSize = 12;
char inputArray[arrSize];
bool incomingData = false;
static int cnt = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  scanBuffer();
  printBuffer();
}

int scanBuffer()
{
  char input;

  while (Serial.available() > 0 && incomingData == false) {
    input = Serial.read();

    if (input != '\n')
    {
      inputArray[cnt] = input;
      cnt++;
      if (cnt >= arrSize) {
        cnt = arrSize - 1;
      }
      return;
    }
    inputArray[cnt] = '\0';
    cnt = 0;
    incomingData = true;

  }


}

void printBuffer()
{
  if (incomingData == true)
  {
    Serial.print("Your input is: ");
    Serial.println(inputArray);
    incomingData = false;
  }
}
