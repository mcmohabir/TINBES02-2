#ifndef COMMANDS_H
#define COMMANDS_H

const int arrSize = 12;
const int stubArgSize = 35;
const int stupDescSize = 65;

typedef struct commandStub
{
    char stubName[stubArgSize];
    char stubDescription[stupDescSize];
    int stubID;

}stub;

typedef struct inputCommands
{
    char commandName[arrSize];
    void *funPtr; //functionpointer
    int commandID;
}commands;

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
    {"store, args: filename, size, data", " Stores the program into EEPROM", 1},
    {"retreive, args: filename", " Retreives the program from EEPROM", 2},
    {"erase, args: filename ", " Erases the program from EEPROM", 3},
    {"files", " Prints a list of the files that are stored in the file system", 4},
    {"freespace", " Prints the available free space that the file system contains", 5},
    {"run, args: filename", " Executes a program", 7},
    {"list", " Prints a list of processes that are currently being executed", 7},
    {"suspend, args: pID", " Suspends a process", 8},
    {"resume, args: pID"," Resumes a suspended process",  9},
    {"kill, args: pID,", " Kills a process", 10}
};



#endif
