#include "memory.h"

#include "instruction_set.h"
stack Stack;

bool memory::storeEntry(byte name, int processID, stack::_stack* stack) {

  if (noOfVars >= TABLE_SIZE)
    return false;   // Memory full

  int inMemory = existsInMemory(name, processID);
  if (inMemory != -1)
    deleteEntry(inMemory);

  byte type = Stack.popByte(stack); // Type next on stack
  int size = getSize(type, stack);

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
    byte data = Stack.popByte(stack);
    memory[memTable[noOfVars].address + i] = data;
    // Serial.println(data);
  }

  ++noOfVars;
  return true;
}

int memory::getVar(byte name, int processID, stack::_stack* stack)
{
	int index = existsInMemory(name, processID);
	if(index < 0 || index > noOfVars)
		return -1;

	for (int i = 0; i < memTable[index].size; i++)
		Stack.pushByte(stack, memory[memTable[index].address + i]);

	if(memTable[index].type == STRING)
		Stack.pushByte(stack, memTable[index].size);

	Stack.pushByte(stack, memTable[index].type);

	return 1;

}
int memory::getSize(char type, stack::_stack* stack)
{
  switch (type)
  {
    case INT:
      return 2;
    case FLOAT:
      return 4;
    case CHAR:
      return 1;
    case STRING:
      return Stack.popByte(stack);   //size next on stack
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
  if (noOfVars == 0 && size <= MEM_SIZE)
        return 0; // Write to first address

    for (byte i = 0; i < noOfVars-1; i++)
    {
        // Check if there is enough space between variables
        if (memTable[i+1].address - (memTable[i].address + memTable[i].size)
        >= size)
            return memTable[i].address + memTable[i].size;
    }

    // Check space after last variable
    if (MEM_SIZE - (memTable[noOfVars-1].address + memTable[noOfVars-1].size)
    >= size)
        return memTable[noOfVars-1].address + memTable[noOfVars-1].size;

    return -1; // No space was found


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
      case INT:
        Serial.print(int(word(memory[memTable[i].address],
                              memory[memTable[i].address + 1])));
        break;

      case CHAR:
        Serial.print(char(memory[memTable[i].address]));
        break;

      case STRING:
        char string[memTable[i].size + 1];
        for (int x = 0; x < memTable[i].size; x++)
          string[x] = char(memory[memTable[i].address + x]);
        Serial.print(string);
        break;

      case FLOAT:
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
