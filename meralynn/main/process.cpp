#include "process.h"

process::process()
{
  noOfProcesses = 0;
}

//==============================================================================
bool process::startProcess(char* name)
{
	if(noOfProcesses >= MAX_PROCESSES)
	{
	  Serial.println(F("ERROR: Processing table full"));
	  return false;
	}

	int fileIndex = fat::existsInFAT(name);
	if(fileIndex == -1)
	{
		Serial.println(F("ERROR: File does not exist"));
		return false;
  	}

	proc process;
	strcpy(process.name, name);
	process.processID = noOfProcesses;
	process.procCtr = fat::getStartPos(fileIndex);
	process.stackPtr = 0;
	process.state = 'r';
	procTable[noOfProcesses] = process;

	noOfProcesses++;

	Serial.print(F("Started: "));
	Serial.println(process.name);
	return true;
}

bool process::changeProcessState(int id, char state)
{
	int processID = processExists(id);
	if(processID == -1)
	{
		Serial.print(F("ERROR: Process does not exist"));
		return false;
	}

	bool stateChanged = setState(processID, state);
	if(!stateChanged)
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
	if(process.state == newState)
		return false;

	process.state = newState;
	procTable[procID] = process;
	Serial.print(F("Changed state to: "));
	Serial.println(process.state);
	return true;
}

int process::processExists(int id)
{
	for (byte i = 0; i < MAX_PROCESSES; i++)
	{
		proc process = procTable[i];
		if (process.processID == id)
			return i;
	}
	return -1;
}


bool process::processList()
{
	for (byte i = 0; i < MAX_PROCESSES; i++)
	{
		proc process = procTable[i];
		if(process.state != '\0')
		{
			Serial.print(F("Process: "));
			Serial.print(process.processID);
			Serial.print(F(" - "));
			Serial.print(process.state);
			Serial.print(F(" - "));
			Serial.println(process.name);
		}
	}
	return true;
}
