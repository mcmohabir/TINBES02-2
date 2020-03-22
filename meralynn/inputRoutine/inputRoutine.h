#ifndef INPUTROUTINE_H
#define INPUTROUTINE_H

#define MAX_COMMAND_ARGS 3
const int arrSize = 12;
static int cnt    = 0;

char** curArgs = new char*[MAX_COMMAND_ARGS];
char* curCommandBuf = new char[arrSize];
//char curArgBuf[arrSize];

char inputCmd[arrSize];
char inputArg[30];

const int infoArgSize = 10;
const int infoParSize = 35;
const int infoDescSize = 65;

int curArgIter = 0;

void printBuffer();
void assignCommand();
void store();
void retreive();
void erase();
void files();
void freespace();
void run ();
void list();
void suspend();
void resume();
void kill();


typedef struct {
  char name[arrSize];
  void *func;
  int commandID;
  char fileName[arrSize];
  int size;
  int processID;
} commandType;

typedef struct {
  char name[infoArgSize];
  char parameters[infoParSize];
  char desc[infoDescSize];
  int id;
} info;

static commandType commandArray[] = {
  {"store", &store},
  {"retreive", &retreive},
  {"erase", &erase},
  {"files", &files},
  {"freespace", &freespace},
  {"run", &run},
  {"list", &list},
  {"suspend", &suspend},
  {"resume", &resume},
  {"kill", &kill}
};

/*
static info commandInfo[] =
{
  {"store", "filename, size, data", "Stores the program into EEPROM", 1},
  {"retreive", "filename", "Retreives the program from EEPROM", 2},
  {"erase", "filename", "Erases the program from EEPROM", 3},
  {"files", "", "Prints a list of the files that are stored in the file system", 4},
  {"freespace", "", "Prints the available free space that the file system contains", 5},
  {"run", "filename", " Executes a program", 6},
  {"list", "", "Prints a list of processes that are currently being executed", 7},
  {"suspend", "pID", " Suspends a process", 8},
  {"resume", "pID", " Resumes a suspended process",  9},
  {"kill", "pID,", " Kills a process", 10}
};
*/
#endif
