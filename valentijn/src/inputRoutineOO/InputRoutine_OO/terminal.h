#ifndef TERMINAL_H
#define TERMINAL_H

#include "Arduino.h"

#define MAX_ARGS 3
#define arrSize 12


class terminal
{
public:
    terminal();
    void initalizeTerminal();

private:
    char **currentArguments = new char *[MAX_ARGS];
    char *currentCommandBuffer = new char[arrSize];
    static int commandCounter = sizeof(availableCommands) / sizeof(command);

    bool incomingData = false;
    bool initialCommand = false;
    bool commandRecognized = false;

    int currentArgIter = 0;

    int scanBuffer();
    void assignCommand(char **arguments);
    bool writeCommand(char inputChar) bool writeArgument(char inputChar);
    char *chrcat(char *appendToChar, char whatTo);
    void store(char **arguments);
    void retreive(char **arguments);
    void erase(char **arguments);
    void files(char **arguments);
    void freespace(char **arguments);
    void run(char **arguments);
    void list(char **arguments);
    void suspend(char **arguments);
    void resume(char **arguments);
    void kill(char **arguments);
    void printBufferArray();
    void printInfo();
    void reset();
    typedef void(terminal::*commandFun)(char** arguments)

    typedef struct commandStruct
    {
        char name[arrSize];
        terminal::commandFun funPtr;
        int cID;
        char fileName[arrSize];
        int size;
        int pID
    } command;
    
    static command availableCommands[];
/*
    static command commandIndex[] =
    {
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
};

*/

#endif