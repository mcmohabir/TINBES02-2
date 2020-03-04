#include "commands.h"


char inputArray[arrSize];

bool commandRecognized = false;
bool incomingData = false;


static int commandIndex = sizeof(availableCommands) / sizeof(commands);
static int stubIndex = sizeof(availableCommandStubs) / sizeof(stub);


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

      if (cnt >= arrSize)
      {
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
      
      Serial.println((String)"Input '" + inputArray + "'  not recognized! Available commands are: \n");
      printStub();
      incomingData  = false;
    }
}



void printStub()
{
    for(int i = 0; i < stubIndex; i++)
    {
        //printfSerial.printf(availableCommandStubs[i].stubName);
        Serial.print(availableCommandStubs[i].stubName);
        Serial.print("\n");
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
