#include "stack.h"

stack::stack()
{

}

// ===============================================================
// Byte
bool stack::pushByte(byte elem) {
  if (sp >= STACKSIZE)
    return false;

  _stack[sp++] = elem;
  return true;
}


byte stack::popByte() {
  if (sp > 0)
    return _stack[sp--];

  return NULL;
}


// ===============================================================
// Char
bool stack::pushChar(char elem)
{
  pushByte(elem);
  pushByte('C');
  return true;
}

char stack::popChar()
{
  return char(popByte());
}


// ===============================================================
// Int
bool stack::pushInt(int elem)
{
  pushByte(highByte(elem));
  pushByte(lowByte(elem));
  pushByte('I');
  return true;
}

int stack::popInt()
{
  byte low = popByte();
  byte high = popByte();
  int num = word(high, low);
  return num;
}


// ===============================================================
// Float
bool stack::pushFloat(float elem)
{
  byte *b = (byte *)&elem;    //float seperated into 4 bytes

  for (int i = 3; i >= 0; i--)
    pushByte(b[i]);

  pushByte('F');
  return true;
}

float stack::popFloat()
{
  float f = 0.0;
  byte *b = (byte *)&f;

  for (int i = 3; i >= 0; i--)
    b[i] = popByte();

  return f;
}


// ===============================================================
// String
bool stack::pushString(char* elem)
{
  byte length = strlen(elem) + 1;
  for (byte i = 0; i < length; i++)
    pushByte(elem[i]);

  pushByte(length);
  pushByte('S');
}

char* stack::popString()
{
  byte length = popByte();
  char s[length]; //+1 = terminating zero

  for (int i = 0; i < length; i++)
    s[i] = popByte();
}

// ===============================================================
// Helpers
float stack::peek()
{
  peeked = true;
  return popVal();
}


float stack::popVal()
{
  byte type = popByte();
  switch (type)
  {
    case 'C':
      char c = popChar();
      if (peeked)
      {
        pushChar(c);
        peeked = false;
      }
      return (float) c;

    case 'I':
      int i = popInt();
      if (peeked)
      {
        pushInt(i);
        peeked = false;
      }
      return (float) i;

    case 'F':
      float f = popFloat();
      if (peeked)
      {
        pushFloat(f);
        peeked = false;
      }
      return (float) f;

    default:
      byte b = popByte();
      if (peeked)
      {
        pushByte(b);
        peeked = false;
      }
      return (float) b;

  }
}
