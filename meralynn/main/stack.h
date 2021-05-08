#ifndef STACK_H
#define STACK_H

#pragma once
#include "Arduino.h"


#define STACKSIZE 32
class stack
{
  public:
	  struct _stack {
  	  byte stack[STACKSIZE];
  	  uint_least8_t sp = 0;
  	};

    bool pushByte(_stack* stack, byte elem);
    byte popByte(_stack* stack);
	float popVal(_stack* stack);
    int procID;


    bool peeked = false;

    bool pushChar(_stack* stack, char elem);
    bool pushInt(_stack* stack, int elem);
    bool pushFloat(_stack* stack, float elem);
    bool pushString(_stack* stack, char* elem);

    char popChar(_stack* stack);
    int popInt(_stack* stack);
    float popFloat(_stack* stack);
    char* popString(_stack* stack);


    float peek(_stack* stack);
};

#endif
