#include "inputRoutine.h"

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
  storeArguments();
  assignCommand();
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


int store(char name, int size)
{
    Serial.println("in store function");
    char printName = name;
    int printSize = (int)size;
    Serial.println(printName);
    Serial.println(printSize);


}


void retreive()
{
  Serial.println("in retreive function");
}

int writeFATEntry()
{
  byte presentFiles;

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
