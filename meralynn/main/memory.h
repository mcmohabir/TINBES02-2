#ifndef MEMORY_H
#define MEMORY_H

#pragma once
#include "Arduino.h"
#include "stack.h"


#define TABLE_SIZE 25
#define MEM_SIZE 256

class memory
{
  public:
    bool initMemory();
    bool printMemTable();
    bool storeEntry(byte name, int processID, stack::_stack* stack);
	int getVar(byte name, int processID, stack::_stack* stack);
	typedef struct {
      byte name;
      int processID;
      char type;
      unsigned int address;
      unsigned int size;
    } memVar;
	memVar memTable[TABLE_SIZE];
	byte memory[MEM_SIZE];

  private:
    int noOfVars = 0;

    int existsInMemory(byte name,  int processID);
    bool deleteEntry(byte index);
    bool pushToStack(byte name, int processID);
    bool clearVars(int processID);
    int getSize(char type,stack::_stack* stack);
    int getStartPos(int size);
    int getNextStartPos(int index);

};

#endif
