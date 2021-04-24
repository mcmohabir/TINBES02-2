#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#pragma once
#include "Arduino.h"
#include "process.h"

class instruction
{
  public:
    static bool execute(int id);
    static bool suspend(int id);
    static bool kill(int id);
};

#endif
