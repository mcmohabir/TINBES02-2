#include "stack.h"

stack::stack()
{

}


bool stack::pushByte(byte elem) {
  if (sp >= STACKSIZE)
    return false;

  stack[sp++] = elem;
  return true;
}


byte stack::popByte() {
  if (sp > 0)
    return stack[sp--];

  return NULL;
}


bool stack::pushInt(int elem)
{
  if (sp <= 0)
    return false;

  char type = 'I';

  pushByte(lowByte(elem));
  pushByte(highByte(elem));
  pushByte(type);

  return true;
}


bool stack::pushFloat(float elem)
{
  if (sp <= 0)
    return false;

  char type = 'F';
  byte *b = (byte *)&elem;    //float seperated into 4 bytes

  for (int i = 4; i > 0; i--)
    pushByte(b[i - 1]);

  pushByte(type);

  return true;
}
