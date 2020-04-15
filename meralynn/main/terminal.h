#ifndef TERMINAL_H
#define TERMINAL_H

#pragma once 
#include "Arduino.h"
#include "FAT.h"

#define MAX_COMMAND_ARGS 3
#define arrSize 12

class terminal
{
  public:
    terminal();
    void execTerminal();

  private:
    char** curArgs = new char*[MAX_COMMAND_ARGS]; // Arguments buffer
    char* curCommandBuf = new char[arrSize];      // Command buffer

    // Variables
    bool incomingData = false;
    bool firstCommand = false;
    bool knownCmd = false;
    int curArgIter = 0;         //Arguments counter

    // Command line functions
    int scanBuffer();
    void assignCommand(char** args);

    // Writing helper functions
    bool writeCommand(char inputChar);
    bool writeArg(char inputChar);
    char* chrcat(char* appendTo, char what);

    void createFAT(char** args);
    void printInput();
    void printInfo();
    void reset();

    // Command functions
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


    typedef void (terminal::*commandFunction)(char** args);
    
    typedef struct {
      char name[arrSize];
      terminal::commandFunction func;
      byte amountArgs;
    } commandType;
    
    static commandType commandArray[];

};
#endif
