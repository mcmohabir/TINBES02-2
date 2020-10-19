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
    bool storeEntry(byte name, int processID);
    bool printMemory();

  private:
    int noOfVars = 0;

    typedef struct {
      byte name;
      char type;
      byte address;
      byte size;
      int processID;
    } memVar;

    memVar memTable[TABLE_SIZE];


    int existsInMemory(byte name,  int processID);
    bool deleteEntry(byte index);
    bool pushToStack(byte name, int processID);
    //    popFromStack();

};

#endif
