#include "memory.h"

stack stack;

bool memory::initMemory()
{
  /*
    memTable[0].name = "a";
    memTable[0].processID = 1;
  */
}


bool memory::printMemTable()
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
    return false;   // Memory full

  int inMemory = existsInMemory(name, processID);
  if (inMemory != -1)
    deleteEntry(inMemory);

  char type = stack.popByte(); // Type next on stack
  int size = getSize(type);

  memVar memEntry;
  memEntry.name = name;
  memEntry.processID = processID;
  memEntry.type = type;
  //  memEntry.address =
  memEntry.size = size;

  memTable[noOfVars] = memEntry;
  ++noOfVars;

  return true;
}


int memory::getSize(char type)
{
  switch (type)
  {
    case 'I':
      return 2;
    case 'F':
      return 4;
    case 'C':
      return 1;
    case 'S':
      return stack.popByte();   //size next on stack
  }
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

  memTable[index] = memVar{};

  noOfVars--;
  return true;
}


int memory::getStartPos(int size)
{
  int firstFreeAddr = 0;
  if (noOfVars == 0)
    return firstFreeAddr;

  bool firstAddr = true;

  for (byte i = 0; i < TABLE_SIZE; i++)
  {
    memVar elem = memTable[i];

    if (elem.size <= 0) break;

    if (firstAddr && size < elem.address)
      return firstFreeAddr;

    if (size < getNextStartPos(i) - (elem.address + elem.size))
      return elem.address + elem.size;

    firstAddr = false;
  }
  
}


int memory::getNextStartPos(int i)
{
  for (byte n = i + 1; n < TABLE_SIZE; n++)
  {
    memVar elem = memTable[n];

    if (elem.size > 0)
      return elem.address;
  }

  return MEM_SIZE; // End of memory
}
