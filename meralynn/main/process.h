#ifndef PROCESS_H
#define PROCESS_H

#pragma once
#include "Arduino.h"

#define MAX_PROCESSES 10
#define MAX_NAME_SIZE 12
class process
{
  public:
    process();

  bool startProcess(char** name);

  private:
  int noOfProcesses;



  
  typedef struct {
    char* name[MAX_NAME_SIZE];
    int processId;
    char state;
    byte procCtr;
    byte filePtr;
    byte stackPtr;
    byte address;
    
  }proc;

  proc procTable[MAX_PROCESSES];

  
};


#endif
