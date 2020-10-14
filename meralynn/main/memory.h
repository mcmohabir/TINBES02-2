#ifndef MEMORY_H
#define MEMORY_H

#pragma once
#include "Arduino.h"

#define TABLE_SIZE 25
#define MEM_SIZE 256

class memory
{
  public:
    int storeEntry(byte name, int processID);
    bool printMemory();
    
  private:
    int noOfVars;

    typedef struct {
      char name;
      char type;
      byte address;
      byte size;
      unsigned int processID;
    } memVar;

     memVar memTable[TABLE_SIZE];


    int existsInMemory(byte name, int processID);
    bool deleteEntry(byte index);
};

#endif
