#ifndef TERMINAL_H
#define TERMINAL_H

#include "Arduino.h"

#define MAX_COMMAND_ARGS 3
#define arrSize 12

class terminal
{
  public:
    terminal();
    void execTerminal();

  private:

    char** curArgs = new char*[MAX_COMMAND_ARGS];
    char* curCommandBuf = new char[arrSize];

    bool incomingData = false;
    bool firstCommand = false;
    bool knownCmd = false;

    int curArgIter = 0;
    int scanBuffer();
    void assignCommand(char** args);

    bool writeCommand(char inputChar);
    bool writeArg(char inputChar);
    char* chrcat(char* appendTo, char what);

    void store(char** args);
    void retreive(char** args);
    void erase(char** args);
    void files(char** args);
    void freespace(char** args);
    void run (char** args);
    void list(char** args);
    void suspend(char** args);
    void resume(char** args);
    void kill(char** args);

    void printBufferArray();
    void printInfo();
    void reset();

    typedef void (terminal::*commandFunction)(char** args);
    typedef struct {
      char name[arrSize];
      terminal::commandFunction func;
      int commandID;
      char fileName[arrSize];
      int size;
      int processID;
    } commandType;

    static commandType commandArray[];
    //static int commandArraySize = sizeof(commandArray) / sizeof(commandType);



};
#endif
