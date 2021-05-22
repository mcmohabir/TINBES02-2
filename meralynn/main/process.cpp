
#include "process.h"
#include "memory.h"
#include "instruction.h"
#include "instruction_set.h"
#include "FAT.h"


memory memory;
instruction instruction;

process::process()
{
  noOfProcesses = 0;
}

bool process::execute(int index)
{
	byte nextInstruction = EEPROM[procTable[index].procCtr++];

	// Serial.print(procTable[index].processID, DEC);
    // Serial.print(F(": "));
    // Serial.print(procTable[index].procCtr-1, DEC);
    // Serial.print(F(": "));
    // Serial.println(nextInstruction, DEC);

	switch (nextInstruction)
    {
        // Datatpyes
        case (CHAR):
        case (INT):
        case (STRING):
        case (FLOAT):
            instruction::valToStack(procTable+index, nextInstruction);
            break;

        // Variables
        case (SET):
            memory.storeEntry(EEPROM[procTable[index].procCtr++], // Name
                            procTable[index].processID, // ProcessID
                            &(procTable[index].stack)); // Stack pointer
            break;
        case (GET):
            memory.getVar(EEPROM[procTable[index].procCtr++], // Name
                            procTable[index].processID, // ProcessID
                            &(procTable[index].stack)); // Stack pointer
            break;

        // Unary operations
        case (INCREMENT):
        case (DECREMENT):
        case (UNARYMINUS):
        case (ABS):
        case (SQ):
        case (SQRT):
        case (ANALOGREAD):
        case (DIGITALREAD):
        case (LOGICALNOT):
        case (BITWISENOT):
        case (TOCHAR):
        case (TOINT):
        case (TOFLOAT):
        case (ROUND):
        case (FLOOR):
        case (CEIL):
            instruction::unaryOp(procTable+index, nextInstruction);
            break;

		// Binary operations
		case (PLUS):
		case (MINUS):
		case (TIMES):
		case (DIVIDEDBY):
		case (MODULUS):
		case (EQUALS):
		case (NOTEQUALS):
		case (LESSTHAN):
		case (LESSTHANOREQUALS):
		case (GREATERTHAN):
		case (GREATERTHANOREQUALS):
		case (MIN):
		case (MAX):
		case (POW):
		case (LOGICALAND):
		case (LOGICALOR):
		case (LOGICALXOR):
		case (BITWISEAND):
		case (BITWISEOR):
		case (BITWISEXOR):
		   instruction::binaryOp(procTable+index, nextInstruction);
		   break;

	   case (IF):
	   case (ELSE):
	   case (ENDIF):
	   case (LOOP):
	   case (ENDLOOP):
	   case (WHILE):
	   case (ENDWHILE):
	   		instruction::branchOp(procTable+index, nextInstruction);
			break;

	   // Time operations
	   case (DELAY):
	   case (DELAYUNTIL):
	   case (MILLIS):
	   		instruction::timeOp(procTable+index, nextInstruction);
			break;

		// Arduino operations
        case (CONSTRAIN):
        case (MAP):
        case (PINMODE):
        case (DIGITALWRITE):
        case (ANALOGWRITE):
            instruction::arduinoOp(procTable+index, nextInstruction);
            break;


        // Print
        case (PRINT):
            instruction.print(procTable+index);
            break;
        case (PRINTLN):
            instruction.print(procTable+index, true); // Add newline
            break;

        // Terminating
        case (STOP):
            setState(procTable[index].processID, 't');
            return true; // Terminate process
        default:
            Serial.print(F("unknown instruction: "));
            Serial.println(nextInstruction, DEC);
            setState(procTable[index].processID, 't');
            return true; // Terminate process
    }

    return false; // Program still running (found no STOP)

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

	unsigned long startTime = 0;
    unsigned long runTime = ceil(50 / (double)nrOfRunningProcesses);

  for (byte i = 0; i < MAX_PROCESSES; i++)
  {
	  startTime = millis();
    while (procTable[i].state == 'r' && (millis() - startTime < runTime))
	{
	bool terminated = execute(i);
	  if (terminated)
		  {
			  i--;
			  break; // Goto next program in for loop
		  }
	  }
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
  process.stack = {};
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
  // Serial.print(F("Changed state to: "));
  // Serial.println(process.state);
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
  process.stack = {};
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
