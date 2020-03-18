#include "inputRoutine.h"
#include "functions.h"


static int commandArraySize = sizeof(command) / sizeof(commandType);
//static int stubArraySize = sizeof(commandInfo) / sizeof(info);

bool incomingData = false;
bool knownCmd = false;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  scanBuffer();
  assignCommand();
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

void printInfo()
{
  for (int i = 0; i < commandArraySize; i++)
    {
      Serial.println(command[i].name);
    }

    Serial.print("\n");
}


/*
   Check if input matches an existing command and assign appropriate function
*/
void assignCommand()
{
  
  if (incomingData)
  {
    for (int i = 0; i < commandArraySize; i++)
    {
      if (strcmp(inputArray, command[i].name) == 0)
      {
        void (*func)() = command[i].func;
        func();
        knownCmd = true;
        break;
      }
    }

    if (knownCmd)
    {
      incomingData = false;
      knownCmd = false;
      return;
    }

    Serial.println((String)"Command '" + inputArray + "' not recognised. These are the available commands: \n");
    printInfo();
    incomingData = false;
  }
}

void readFATEntry()
{

}


void writeFATEntry(struct function f)
{
  EEPROM.put(noOfFiles, f);
  noOfFiles += sizeof(f);
  
}

int fileInFAT(char )
{
  
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
  Serial.println("in erase function");
}


void files()
{
  Serial.println("in files function");
}


void freespace()
{
  Serial.println("in freespace function");
}


void run()
{
  Serial.println("in run function");
}


void list()
{
  Serial.println("in list function");
}


void suspend()
{
  Serial.println("in suspend function");
}


void resume()
{
  Serial.println("in resume function");
}


void kill()
{
  Serial.println("in kill function");
}
