#ifndef PROCESS_H
#define PROCESS_H

#pragma once
#include "Arduino.h"
#include "stack.h"

#define MAX_PROCESSES 10
#define MAX_NAME_SIZE 12
class process
{
  public:
    process();
    void runPrograms();
    bool startProcess(char* name);
    bool changeProcessState(int id, char state);
    bool processList();
	bool execute(int index);
    typedef struct {
      char name[MAX_NAME_SIZE];
      int processID;
      char state;
      int procCtr;
      byte filePtr;
      // byte stackPtr;
	  stack::_stack stack;
      byte address;
    } proc;
    proc procTable[MAX_PROCESSES];

  private:
    int noOfProcesses;

    int processExists(int id);
    bool setState(int procID, char newState);
    bool killProcess(int procID);




};


#endif
