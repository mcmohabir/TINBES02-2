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
    void assignCommand(char** arguments);

    bool writeCommand(char inputChar);
    bool writeArg(char inputChar);
    char* chrcat(char* appendTo, char what);

    void store(char** arguments);
    void retreive(char** arguments);
    void erase(char** arguments);
    void files(char** arguments);
    void freespace(char** arguments);
    void run (char** arguments);
    void list(char** arguments);
    void suspend(char** arguments);
    void resume(char** arguments);
    void kill(char** arguments);

    void printBufferArray();
    void printInfo();
    void reset();

    typedef struct {
      char name[arrSize];
      void *func;
      int commandID;
      char fileName[arrSize];
      int size;
      int processID;
    } commandType;

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

};
#endif
