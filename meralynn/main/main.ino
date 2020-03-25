#include "terminal.h"


void setup()
{
  Serial.begin(9600);
}


void loop()
{
  terminal terminal;
  terminal.execTerminal;
}
