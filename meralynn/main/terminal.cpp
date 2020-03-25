#include "terminal.h"
#include "FAT.h"


static int commandArraySize = sizeof(commandArray) / sizeof(commandType);


terminal::terminal()
{
  terminal::reset();
}

void terminal::execTerminal()
{
  int commandEntered = scanBuffer();
  if (commandEntered == 0)
    return;

  assignCommand(curArgs);
}

int terminal::scanBuffer()
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
      return 0;
    }

    incomingData = true;
    printBufferArray();
    return 1;
  }

}

/*
   Check if input matches an existing command and assign appropriate function
*/
void terminal::assignCommand(char** arguments)
{
  if (!incomingData)
    return;

  for (int i = 0; i < commandArraySize; i++)
  {
    if (strcmp(curCommandBuf, commandArray[i].name) == 0)
    {
      void (*func)(char**) = commandArray[i].func;
      func(arguments);
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

bool terminal::writeCommand(char inputChar)
{
  if (inputChar == ' ' || inputChar == '\n')
    return true;

  curCommandBuf = chrcat(curCommandBuf, inputChar);
  return false;
}


bool terminal::writeArg(char inputChar)
{
  if (inputChar == ' ' || inputChar == '\n')
    return true;

  curArgs[curArgIter] = chrcat(curArgs[curArgIter], inputChar);
  return false;
}


char* terminal::chrcat(char* appendTo, char what)
{
  byte len = strlen(appendTo);
  if (len == (arrSize - 1)) return appendTo;

  appendTo[len] = what;
  appendTo[len + 1] = 0;
  return appendTo;
}


void terminal::reset()
{
  curCommandBuf[0] = '\0';
  for (byte i = 0; i < MAX_COMMAND_ARGS; i++)
  {
    curArgs[i][0] = '\0';
  }
  curArgIter = 0;
  firstCommand = false;
}

void terminal::printBufferArray()
{
  if (!incomingData)
    return;

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


void terminal::printInfo()
{
  for (int i = 0; i < commandArraySize; i++)
  {
    Serial.println(commandArray[i].name);
  }

  Serial.print("\n");
}


void terminal::store (char** arguments)
{
  Serial.println("in store function");

  //check if vrije entry in FAT
  //functie die checkt of een bestand met naam aanwezig is in FAT
  if (fat::existsInFAT()) {

  }

}


void terminal::retreive(char** arguments)
{
  Serial.println("in retreive function");
}


void terminal::erase(char** arguments)
{
  Serial.println("in erase function");
}


void terminal::files(char** arguments)
{
  Serial.println("in files function");
}


void terminal::freespace(char** arguments)
{
  Serial.println("in freespace function");
}


void terminal::run(char** arguments)
{
  Serial.println("in run function");
}


void terminal::list(char** arguments)
{
  Serial.println("in list function");
}


void terminal::suspend(char** arguments)
{
  Serial.println("in suspend function");
}


void terminal::resume(char** arguments)
{
  Serial.println("in resume function");
}


void terminal::kill(char** arguments)
{
  Serial.println("in kill function");
}
