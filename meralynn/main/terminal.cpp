#include "terminal.h"
memory memory;


// Commands struct array

terminal::commandType terminal::commandArray[] = {
  //{"initFAT",     &terminal::createFAT,   0},
  {"store",       &terminal::store,       2},
  {"retreive",    &terminal::retreive,    1},
  {"erase",       &terminal::erase,       1},
  {"files",       &terminal::files,       0},
  {"freespace",   &terminal::freespace,   0},
  {"run",         &terminal::run,         1},
  {"list",        &terminal::list,        0},
  {"suspend",     &terminal::suspend,     1},
  {"resume",      &terminal::resume,      1},
  {"kill",        &terminal::kill,        1},
  {"getmem",      &terminal::getMem,      0},
  {"storemem",    &terminal::storeMem,    2}
};

//==============================================================================
// Terminal Initialization

terminal::terminal()
{
  curCommandBuf[0] = '\0';
  for (byte i = 0; i < MAX_COMMAND_ARGS; i++)
  {
    curArgs[i] = new char[MAX_ARG_SIZE];
    curArgs[i][0] = '\0';
  }

  fat::initFAT();

}


void terminal::execTerminal()
{
  int commandEntered = scanBuffer();
  if (commandEntered == 0) return false;

  assignCommand(curArgs);
  //  reset();

}


//==============================================================================
// Reset
void terminal::reset()
{
  curCommandBuf[0] = '\0';
  for (byte i = 0; i < MAX_COMMAND_ARGS; i++)
    curArgs[i][0] = '\0';
  curArgIter = 0;
  firstCommandSet = false;
}


//==============================================================================
//== Command line

// Scan command line and put input in correct buffers
int terminal::scanBuffer()
{
  char input;

  while (Serial.available() > 0 && incomingData == false)
  {
    input = Serial.read();

    if (input != '\n')
    {
      if (!firstCommandSet)
        firstCommandSet = writeCommand(input);

      else {
        curArgIter += writeArg(input);
      }
      return 0;
    }

    incomingData = true;
    printInput();
    return 1;
  }
}

// Check if input matches existing commmand and go to corresponding function
void terminal::assignCommand(char** args)
{
  // No input to process
  if (!incomingData)
    return false;

  for (int i = 0; i < (sizeof(commandArray) / sizeof(commandType)); i++)
  {
    if (strcmp(curCommandBuf, commandArray[i].name) == 0)
    {
      commandFunction func = commandArray[i].func;
      (this->*func)(args);
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
  printCommandArray();
  incomingData = false;
  reset();
}


//==============================================================================
//== Writing helper functions

bool terminal::writeCommand(char inputChar)
{
  if (inputChar == ' ' || inputChar == '\n')
    return true;

  curCommandBuf = chrcat(curCommandBuf, inputChar, MAX_COMMAND_SIZE);
  return false;
}


bool terminal::writeArg(char inputChar)
{
  if (inputChar == ' ' || inputChar == '\n')
    return true;

  curArgs[curArgIter] = chrcat(curArgs[curArgIter], inputChar, MAX_ARG_SIZE);
  return false;
}


// Add character to buffer
char* terminal::chrcat(char* appendTo, char character, int maxLength)
{
  byte len = strlen(appendTo);
  if (len == (maxLength - 1)) return appendTo;

  appendTo[len] = character;
  appendTo[len + 1] = 0;
  return appendTo;
}

//==============================================================================
//== Print functions

void terminal::printInput()
{
  //  if (!incomingData)
  //    return;

  Serial.print("$ ");
  Serial.print(curCommandBuf);
  for (byte i = 0; i < sizeof(curArgs) + 1; i++)
  {
    Serial.print(" ");
    Serial.print(curArgs[i]);
  }
  Serial.println();
}


void terminal::printCommandArray()
{
  for (int i = 0; i < (sizeof(commandArray) / sizeof(commandType)); i++)
  {
    Serial.println(commandArray[i].name);
  }

  Serial.print("\n");
}

//==============================================================================
// FAT Functions
/*
  void terminal::createFAT(char** args)
  {
  fat::initFAT();
  }
*/

//==============================================================================
//== Command functions

void terminal::store (char** args)
{
  //  Serial.println("in store function");
  int size = strlen(args[1]) + 1;
  if (fat::addFile(args[0], size, args[1]))
  {
    Serial.print("stored ");
    Serial.print(args[0]);
    Serial.print(" with data: ");
    Serial.println(args[1]);
  }
  else
  {
    Serial.println("failed store");
  }
}


void terminal::retreive(char** args)
{
  char* data = fat::readFile(args[0]);
  Serial.println(data);
  delete[] data;
}


void terminal::erase(char** args)
{
  if (fat::deleteFile(args[0]))
  {
    Serial.print(args[0]);
    Serial.println(" succesfully deleted");
  }
  else {
    Serial.println("file nog found");
  }
}


void terminal::files(char** args)
{
  fat::listFiles();
}


void terminal::freespace(char** args)
{
  int freeSpace = fat::freespace();
  if (freeSpace == 0 || freeSpace == -1)
  {
    Serial.println("No free space available");
    return;
  }

  Serial.print(freeSpace);
  Serial.println(" bytes available");
}


void terminal::run(char** args)
{
  Serial.println("in run function");
}


void terminal::list(char** args)
{
  Serial.println("in list function");
}


void terminal::suspend(char** args)
{
  Serial.println("in suspend function");
}


void terminal::resume(char** args)
{
  Serial.println("in resume function");
}


void terminal::kill(char** args)
{
  Serial.println("in kill function");
}

void terminal::getMem(char** args)
{
  memory.printMemory();
}

void terminal::storeMem(char** args)
{
  memory.storeEntry(args[0], args[1]);
}
