#ifndef STACK_H
#define STACK_H

#pragma once
#include "Arduino.h"

#define STACKSIZE 32
class stack
{
  public:
    bool pushByte(byte elem);
    byte popByte();


  private:
    bool pushChar(char elem);
    bool pushInt(int elem);
    bool pushFloat(float elem);
    bool pushString(char elem);

    char popChar();
    int popInt();
    float popFloat();
    char popString();

};

#endif
