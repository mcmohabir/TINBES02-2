
#include "process.h"
#include "memory.h"
#include "instruction.h"
memory memory;
instruction instruction;

process::process()
{
  noOfProcesses = 0;
}

void process::runPrograms()
{
  int nrOfRunningProcesses = 0;
  for (byte i = 0; i < MAX_PROCESSES; i++)
  {
    if (procTable[i].state == 'r')
      nrOfRunningProcesses++;
    if (procTable[i].state == 't')
    {
      bool killed = killProcess(i);
      if (killed)
      {
        Serial.print(F("Process "));
        Serial.print(i);
        Serial.println(F(" succesfully terminated"));
        break;
      }
      Serial.print(F("ERROR: Failed to terminate process"));
      Serial.println(i);
    }
  }
  if (nrOfRunningProcesses == 0)
    return;

  for (byte i = 0; i < MAX_PROCESSES; i++)
  {
    if (procTable[i].state == 'r')
      instruction.execute(i);
  }
}
//==============================================================================
bool process::startProcess(char* name)
{
  if (noOfProcesses >= MAX_PROCESSES)
  {
    Serial.println(F("ERROR: Processing table full"));
    return false;
  }

  int fileIndex = fat::existsInFAT(name);
  if (fileIndex == -1)
  {
    Serial.println(F("ERROR: File does not exist"));
    return false;
  }

  int id;
  for (byte i = 0; i < MAX_PROCESSES; i++)
  {
    if (procTable[i].state == '\0')
    {
      id = i;
      break;
    }
  }

  proc process;
  strcpy(process.name, name);
  process.processID = id;
  process.state = 'r';
  process.stackPtr = 0;
  process.filePtr = 0;
  process.procCtr = fat::getStartPos(fileIndex);

  procTable[id] = process;
  noOfProcesses++;

  Serial.print(F("Started: "));
  Serial.println(process.name);
  return true;
}

bool process::changeProcessState(int id, char state)
{
  int processID = processExists(id);
  if (processID == -1)
  {
    Serial.print(F("ERROR: Process does not exist"));
    return false;
  }

  // if(state == 't')
  // {
  // 	bool killed = killProcess(id);
  // 	if(killed)
  // 	{
  // 		Serial.print(F("Process "));
  // 		Serial.print(id);
  // 		Serial.println(F(" succesfully terminated"));
  // 		return true;
  // 	}
  // 	Serial.print(F("ERROR: Failed to terminate process"));
  // 	Serial.println(id);
  // 	return false;
  // }

  bool stateChanged = setState(processID, state);
  if (!stateChanged)
  {
    Serial.print(F("ERROR: Process "));
    Serial.print(id);
    Serial.print(F(" already in state "));
    Serial.println(state);
    return false;
  }

  return true;
}

//==============================================================================
bool process::setState(int procID, char newState)
{
  proc process = procTable[procID];
  if (process.state == newState)
    return false;


  process.state = newState;
  procTable[procID] = process;
  Serial.print(F("Changed state to: "));
  Serial.println(process.state);
  return true;
}

bool process::killProcess(int procID)
{
  proc process = procTable[procID];
  strcpy(process.name, '\0');
  process.state = '\0';
  process.processID = NULL;
  process.procCtr = NULL;
  process.filePtr = NULL;
  process.stackPtr = NULL;
  process.address = NULL;
  procTable[procID] = process;
  noOfProcesses--;
  // memory.clearVars(procID);
  return true;

}


int process::processExists(int id)
{
  for (byte i = 0; i < MAX_PROCESSES; i++)
  {
    proc process = procTable[i];
    if (process.processID == id && process.state != NULL)
      return i;
  }
  return -1;
}


bool process::processList()
{
  for (byte i = 0; i < MAX_PROCESSES; i++)
  {
    proc process = procTable[i];
    if (process.state != '\0')
    {
      Serial.print(F("Process: "));
      Serial.print(process.processID);
      Serial.print(F(" - "));
      Serial.print(process.state);
      Serial.print(F(" - "));
      Serial.print(process.procCtr);
      Serial.print(F(" - "));
      Serial.println(process.name);
    }
  }
  return true;
}
