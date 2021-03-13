#include "process.h"

process::process()
{
  noOfProcesses = 0;
}


bool process::startProcess(char** name)
{
  if(noOfProcesses >= MAX_PROCESSES)
    Serial.println("Processing table full");

  

  
}
