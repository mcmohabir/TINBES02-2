#ifndef TERMINAL_H
#define TERMINAL_H

#pragma once
#include "Arduino.h"
#include "FAT.h"
#include "memory.h"
#include "process.h"

#define MAX_COMMAND_ARGS 3


class terminal
{
  public:
    terminal();
    void execTerminal();
	void execPrograms();

  private:
    char** curArgs = new char*[MAX_COMMAND_ARGS];         // Arguments buffer
    char* curCommandBuf = new char[MAX_COMMAND_SIZE];     // Command buffer

    // Variables
    bool incomingData = false;
    bool firstCommandSet = false;
    bool knownCmd = false;
    int curArgIter = 0;         //Current Arguments counter

    // Command line functions
    int scanBuffer();
    void assignCommand(char** args);

    // Writing helper functions
    bool writeCommand(char inputChar);
    bool writeArg(char inputChar);
    char* chrcat(char* appendTo, char character, int maxLength);

    void createFAT(char** args);
    void printInput();
    void printCommandArray();
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
    void fatsize(char** args);
    void getMem(char** args);
    void storeMem(char** args);

	void test(char** args);

    typedef void (terminal::*commandFunction)(char** args);

    typedef struct {
      char name[MAX_COMMAND_SIZE];
      terminal::commandFunction func;
      byte amountArgs;
    } commandType;

    static commandType commandArray[];

};
#endif
