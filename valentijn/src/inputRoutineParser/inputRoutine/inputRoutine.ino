#include "commands.h"

bool commandRecognized = false;
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


void assignCommand()
{

    static int commandIndex = sizeof(availableCommands) / sizeof(commands);

    if(incomingData)
    {
        for(int i = 0; i < commandIndex; i++)
        {
            if(strcmp(inputArray, availableCommands[i].commandName) == 0)
            {
                void (*funPtr)() = availableCommands[i].funPtr;
                funPtr();
                commandRecognized = true;

            }
        }

      if(commandRecognized)
      {
          incomingData = false;
          commandRecognized = false;
          return;
      }

        Serial.println("Input not recognized! Available commands are: ");
          for(int i = 0; i < commandIndex; i++)
          {
                Serial.println(availableCommandStubs[i].stubName);
                Serial.println(availableCommandStubs[i].stubDescription);
          }


    }
    incomingData = false;
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

void storeFunc()
{
    Serial.println("store function");
}

void retreiveFunc()
{
    Serial.println("retreive function");
}

void eraseFunc()
{
    Serial.println("erase function");
}


void filesFunc()
{
    Serial.println("files function");
}

void freespaceFunc()
{
    Serial.println("freespace function");
}

void runFunc()
{
    Serial.println("run function");
}

void listFunc()
{
    Serial.println("list function");
}

void suspendFunc()
{
    Serial.println("suspend function");
}


void resumeFunc()
{
    Serial.println("resume function");
}

void killFunc()
{
    Serial.println("kill function");
}
