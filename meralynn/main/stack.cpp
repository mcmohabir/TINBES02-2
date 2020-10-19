#include "stack.h"

byte Stack[STACKSIZE];
byte sp = 0;


bool pushByte(byte elem) {
  if (sp >= STACKSIZE)
    return false;

  Stack[sp++] = elem;
  return true;
}


byte popByte() {
  if (sp > 0)
    return Stack[sp--];

  return NULL;
}


bool pushInt(int elem)
{
  char type = "INT";

  pushByte(lowByte(elem));
  pushByte(highByte(elem));
  pushByte(type);
}


bool pushFloat(float elem)
{
  char type = "FLOAT";
  byte *b = (byte *)&elem;

  for (int i = 4; i > 0; i--)
  {
    pushByte(b[i - 1]);
  }

  pushByte(type);
}
