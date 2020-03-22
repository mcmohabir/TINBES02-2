#include "inputRoutine.h"
#include "functions.h"


static int commandArraySize = sizeof(commandArray) / sizeof(commandType);

void setup()
{
  Serial.begin(9600);
  reset();
}

void loop()
{
  terminal();
}


void terminal()
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
      if (!firstCommand)
        firstCommand = writeCommand(input);

      else {
        curArgIter += writeArg(input);
      }
      return;
    }

    incomingData = true;
    printBufferArray();
  }

}


void assignCommand()
{
  if (incomingData)
  {

    for (int i = 0; i < commandArraySize; i++)
    {
      if (strcmp(curCommandBuf, commandArray[i].name) == 0)
      {
        void (*func)() = commandArray[i].func;
        func();
        knownCmd = true;
        break;
      }
    }

    if (knownCmd)
    {
      incomingData = false;
      knownCmd = false;
      reset();
      return;
    }

    Serial.println((String)"Command '" + curCommandBuf + "' not recognised. These are the available commands: \n");
    printInfo();
    incomingData = false;
    reset();
  }
}


void reset()
{
  curCommandBuf[0] = '\0';
  for (byte i = 0; i < MAX_COMMAND_ARGS; i++)
  {
    curArgs[i][0] = '\0';
  }
  curArgIter = 0;
  firstCommand = false;
}


bool writeCommand(char inputChar)
{
  if (inputChar == ' ' || inputChar == '\n')
    return true;

  curCommandBuf = chrcat(curCommandBuf, inputChar);
  return false;
}


bool writeArg(char inputChar)
{
  if (inputChar == ' ' || inputChar == '\n')
    return true;

  curArgs[curArgIter] = chrcat(curArgs[curArgIter], inputChar);
  return false;
}


char* chrcat(char* appendTo, char what)
{
  byte len = strlen(appendTo);
  if (len == (arrSize - 1)) return appendTo;

  appendTo[len] = what;
  appendTo[len + 1] = 0;
  return appendTo;
}


void printBufferArray()
{
  if (incomingData)
  {
    Serial.print("Your command is: ");
    Serial.println(curCommandBuf);

    Serial.print("Your arguments are: ");
    for (int i = 0; i < sizeof(curArgs) + 1; i++)
    {
      Serial.print(" ");
      Serial.print(curArgs[i]);
    }
    Serial.println();

    
  }
}


/*
   Check if input matches an existing command and assign appropriate function
*/


/*
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
*/

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


void printInfo()
{
  for (int i = 0; i < commandArraySize; i++)
  {
    Serial.println(commandArray[i].name);
  }

  Serial.print("\n");
}
