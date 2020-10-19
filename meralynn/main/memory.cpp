#include "memory.h"


stack stack;


bool memory::initMemory()
{
  /*
    memTable[0].name = "a";
    memTable[0].processID = 1;
  */
}


bool memory::printMemory()
{
  for (byte i = 0; i < TABLE_SIZE; i++)
  {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(char(memTable[i].name));
    Serial.print(", ");
    Serial.println(memTable[i].processID);
  }
  return true;
}


bool memory::storeEntry(byte name, int processID) {

  if (noOfVars >= TABLE_SIZE)
    return false;

  int inMemory = existsInMemory(name, processID);
  if (inMemory != -1)
    deleteEntry(inMemory);

  memVar memEntry;
  memEntry.name = name;
  memEntry.processID = processID;
  memEntry.type = stack.popByte();
//  memEntry.address = 
//  memEntry.size = 

  memTable[noOfVars] = memEntry;
  ++noOfVars;

  return true;

}

int memory::existsInMemory(byte name, int processID)
{
  if (noOfVars <= 0)
    return -1;

  for (byte i = 0; i < noOfVars; i++)
  {
    if (name == memTable[i].name && processID == memTable[i].processID)
      return i;
  }

  return -1;
}

bool memory::deleteEntry(byte index)
{
  while (memTable[index + 1].name != NULL)
  {
    memTable[index] = memTable[index + 1];
    ++index;
  }

  memTable[index].name = NULL;
  memTable[index].type = NULL;
  memTable[index].address = NULL;
  memTable[index].size = NULL;
  memTable[index].processID = NULL;

  noOfVars--;
  return true;
}
