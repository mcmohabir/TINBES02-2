#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#pragma once
#include "Arduino.h"
#include "process.h"

class instruction
{
  public:
	static void valToStack(process::proc* process, byte datatype);
	static void unaryOp(process::proc* process, byte operation);
	static void binaryOp(process::proc* process, byte operation);
	static void timeOp(process::proc* process, byte operation);
	static void print(process::proc* process, bool newline=false);
};

#endif
