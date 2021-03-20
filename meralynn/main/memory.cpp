#include "memory.h"

stack stack;

bool memory::initMemory()
{
  /*
    memTable[0].name = "a";
    memTable[0].processID = 1;
  */
}


bool memory::storeEntry(byte name, int processID) {

  if (noOfVars >= TABLE_SIZE)
    return false;   // Memory full

  int inMemory = existsInMemory(name, processID);
  if (inMemory != -1)
    deleteEntry(inMemory);

  char type = stack.popByte(); // Type next on stack
  int size = getSize(type);

  int address = getStartPos(size);

  memVar memEntry;
  memEntry.name = name;
  memEntry.processID = processID;
  memEntry.type = type;
  memEntry.address = address;
  memEntry.size = size;

  memTable[noOfVars] = memEntry;

  for (int i = (memTable[noOfVars].size - 1); i >= 0; i--)
  {
    byte data = stack.popByte();
    memory[memTable[noOfVars].address + i] = data;
    Serial.println(data);
  }

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

  for (byte i = 0; i < noOfVars; i++)
  {
    memVar elem = memTable[i];

    if (firstAddr && size < elem.address)
      return firstFreeAddr;

    // Space found
    if (size < (getNextStartPos(i) - (elem.address + elem.size)))
      return elem.address + elem.size;

    firstAddr = false;
  }

}


int memory::getNextStartPos(int index)
{
  for (byte n = index + 1; n < TABLE_SIZE; n++)
  {
    memVar elem = memTable[n];

    if (elem.size > 0)
      return elem.address;
  }

  return MEM_SIZE; // End of memory
}


bool memory::printMemTable()
{
  for (byte i = 0; i < TABLE_SIZE; i++)
  {
    if (memTable[i].name == NULL)
      break;

    Serial.print(i);
    Serial.print(F(", name: "));
    Serial.print(memTable[i].name);
    Serial.print(F(", p-id: "));
    Serial.print(memTable[i].processID);
    Serial.print(F(", addr: "));
    Serial.print(memTable[i].address);
    Serial.print(F(", size: "));
    Serial.print(memTable[i].size);
    Serial.print(F(", type: "));
    Serial.print(memTable[i].type);
    Serial.print(F(", data: "));

    switch (memTable[i].type) {
      case 'I':
        Serial.print(int(word(memory[memTable[i].address],
                              memory[memTable[i].address + 1])));
        break;

      case 'C':
        Serial.print(char(memory[memTable[i].address]));
        break;

      case 'S':
        char string[memTable[i].size + 1];
        for (int x = 0; x < memTable[i].size; x++)
          string[x] = char(memory[memTable[i].address + x]);
        Serial.print(string);
        break;

      case 'F':
        float f = 0.0;
        byte* b = (byte*) &f;
        for (int x = 0; x < 4; x++)
        {
          Serial.println(memory[memTable[i].address + x]);
          b[x] = memory[memTable[i].address + x];
        }
        Serial.print(f);
        break;
    }
    Serial.println();
  }
  return true;
}


bool memory::clearVars(int processID)
{
  for (byte i = 0; i < noOfVars; i--)
    if (memTable[i].processID == processID)
    {
      deleteEntry(i);
      i--;
    }
}
