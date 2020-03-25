#include "proc.h"

proc::proc()
{
  
}

void proc::run()
{
  terminal terminal;
  
  while(true)
  {
    terminal.execTerminal();  
  }
}
