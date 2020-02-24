#include "cmdInterface.h"


bool incomingData = false;


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  scanBuffer();
  assignCommand();
  //printBuffer();


}

int scanBuffer()
{
  char input;

  while (Serial.available() > 0 && incomingData == false)
  {
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
  if (incomingData)
  {
    Serial.print("Your input is: ");
    Serial.println(inputArray);
    incomingData = false;
  }
}


void assignCommand()
{
  static int structArraySize = sizeof(command) / sizeof(commandType);
  if (incomingData)
  {
    for (int i = 0; i < structArraySize; i++)
    {
      if (strcmp(inputArray, command[i].commandName) == 0)
      {
        void (*func)() = command[i].func;
        func();
      }
    }
    incomingData = false;
  }
}


void store ()
{
  Serial.println("in store function");
}


void retreive()
{
  Serial.println("in retreive function");
}


void erase()
{

}


void files()
{

}


void freespace()
{

}


void run()
{

}


void list()
{

}


void suspend()
{

}


void resume()
{

}


void kill()
{

}
