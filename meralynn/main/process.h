#ifndef PROCESS_H
#define PROCESS_H

#pragma once
#include "Arduino.h"
#include "FAT.h"

#define MAX_PROCESSES 10
#define MAX_NAME_SIZE 12
class process
{
  public:
    process();

  bool startProcess(char* name);
  bool changeProcessState(int id, char state);
  bool processList();
  int processExists(int id);
  bool setState(int procID, char newState);

  private:
  int noOfProcesses;

  typedef struct {
    char name[MAX_NAME_SIZE];
    int processID;
    char state;
    byte procCtr;
    byte filePtr;
    byte stackPtr;
    byte address;
  }proc;

  proc procTable[MAX_PROCESSES];


};


#endif
