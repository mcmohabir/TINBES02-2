#ifndef COMMANDS_H
#define COMMANDS_H

const int arrSize = 12;
static int cnt = 0;

const int stubArgSize = 10;
const int stubParamSize = 35;
const int stupDescSize = 65;

void printBuffer();
void storeFunc();
void retreiveFunc();
void eraseFunc();
void filesFunc();
void freespaceFunc();
void runFunc();
void listFunc();
void suspendFunc();
void resumeFunc();
void killFunc();



typedef struct inputCommands
{
    char commandName[arrSize];
    void *funPtr; //functionpointer
    // int commandID;
}commands;

typedef struct commandStub
{
    char stubName[stubArgSize];
    char stubParams[stubParamSize];
    char stubDescription[stupDescSize];
    int stubID;

}stub;


static commands availableCommands[] =
{
    {"store",&storeFunc},
    {"retreive",&retreiveFunc},
    {"erase",&eraseFunc},
    {"files",&filesFunc},
    {"freespace",&freespaceFunc},
    {"run",&runFunc},
    {"list", &listFunc},
    {"suspend",&suspendFunc},
    {"resume",&resumeFunc},
    {"kill",&killFunc}
};


static stub availableCommandStubs[] =
{
    {"store", "filename, size, data", " Stores the program into EEPROM", 1},
    {"retreive", "filename", " Retreives the program from EEPROM", 2},
    {"erase", "filename", " Erases the program from EEPROM", 3},
    {"files", " ", "Prints a list of the files that are stored in the file system", 4},
    {"freespace", " ", "Prints the available free space that the file system contains", 5},
    {"run", "filename", " Executes a program", 6},
    {"list", " ", "Prints a list of processes that are currently being executed", 7},
    {"suspend", "pID", " Suspends a process", 8},
    {"resume", "pID"," Resumes a suspended process",  9},
    {"kill", "pID", "Kills a process", 10}
};


#endif
