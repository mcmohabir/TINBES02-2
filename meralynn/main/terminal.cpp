#include "terminal.h"


terminal::commandType terminal::commandArray[] = {
  {"store", &terminal::store},
  {"retreive", &terminal::retreive},
  {"erase", &terminal::erase},
  {"files", &terminal::files},
  {"freespace", &terminal::freespace},
  {"run", &terminal::run},
  {"list", &terminal::list},
  {"suspend", &terminal::suspend},
  {"resume", &terminal::resume},
  {"kill", &terminal::kill}
};

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
void terminal::assignCommand(char** args)
{
  if (!incomingData)
    return;

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
  for (int i = 0; i < (sizeof(commandArray) / sizeof(commandType)); i++)
  {
    Serial.println(commandArray[i].name);
  }

  Serial.print("\n");
}

void terminal::createFAT(char** args)
{
    fat::initFAT(); 
}

bool fat::listFiles()
{
  for (byte i = 0; i < FAT_SIZE; i++)
    {
        file eepromfile = readFATEntry(i);
        if (file.size > 0) // Only print files that contain data
        {
            Serial.print("file: ");
            Serial.print(i);                            // File index from FAT
            Serial.print("\t- ");
            Serial.print(eepromfile.name);                    // Filename
            Serial.print("\t\t");
            Serial.print(eepromfile.beginsPos);                // Start byte on disk
            Serial.print("/");
            Serial.print(eepromfile.beginPos + file.length);    // End byte on disk
            Serial.print("\t(");
            Serial.print(eepromfile.length);                    // Filesize
            Serial.println(" bytes)");
        }
    }
    return true;
}

void terminal::store (char** args)
{
  Serial.println("in store function");
  int size = strlen(args[1]) + 1;
  if (fat::addFile(args[0], size, args[1]))
  {
    Serial.print("stored: ");
    Serial.println(args[0]);
  }
  else
  {
    Serial.println("failed store");
  }
}


void terminal::retreive(char** args)
{
  Serial.println("in retreive function");
  char* data = fat::readFile(args[0]);
  delete[] data;
}


void terminal::erase(char** args)
{
  Serial.println("in erase function");
  if(fat::deleteFile(args[0]))
  {
    Serial.print("deleted file: ");
    Serial.println(args[0]);
  }
  else{
    Serial.println("file nog found")
  }
}


void terminal::files(char** args)
{
  Serial.println("in files function");
  fat::listFiles();
}


void terminal::freespace(char** args)
{
  Serial.println("in freespace function");
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
