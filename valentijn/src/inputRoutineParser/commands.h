#ifndef COMMANDS_H
#define COMMANDS_H

const int arrSize = 12;

typedef struct commandStub
{
    int stubID;
    char stubName[arrSize];
    char stubDescription[50];
}stub;

typedef struct inputCommands
{
    char commandName[arrSize];
    void *funPtr; //functionpointer
    int commandID;
    stub commandStub;

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



#endif
