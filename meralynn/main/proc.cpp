#include "proc.h"

proc::proc()
{

}

void proc::run()
{
  terminal terminal;
	Serial.println(F("-- Ready to go! --"));
  while (true)
  {
    terminal.execTerminal();
    terminal.execPrograms();
  }

}
