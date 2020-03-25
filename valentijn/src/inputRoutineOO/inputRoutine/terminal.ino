#include "terminal.h"

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
        void (*func)(char, char, char) = commandArray[i].func;
        func(argVal1, argVal2, argVal3);
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


void storeArguments()
{
    argVal1 = curArgs[1];
    argVal2 = curArgs[2];
    argVal3 = curArgs[3];
}