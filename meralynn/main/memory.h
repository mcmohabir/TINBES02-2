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
    bool storeEntry(byte name, int processID);

  private:
    int noOfVars = 0;

    typedef struct {
      byte name;
      int processID;
      char type;
      byte address;
      byte size;
    } memVar;

    memVar memTable[TABLE_SIZE];
    byte memory[MEM_SIZE];

    int existsInMemory(byte name,  int processID);
    bool deleteEntry(byte index);
    bool pushToStack(byte name, int processID);
    bool clearVars(int processID);
    int getSize(char type);
    int getStartPos(int size);
    int getNextStartPos(int index);

};

#endif
