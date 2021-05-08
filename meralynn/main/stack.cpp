#include "stack.h"
#include "instruction_set.h"
// ===============================================================
// Byte
bool stack::pushByte(_stack* stack, byte elem) {
  stack->stack [(stack->sp)++] = elem;
  return true;
}


byte stack::popByte(_stack* stack) {

  // if(peek) return stack->stack[(stack->sp)-1];
  return stack->stack [--(stack->sp)];
}


// ===============================================================
// Char
bool stack::pushChar(_stack* stack, char elem)
{
  pushByte(stack, elem);
  pushByte(stack, CHAR);
  return true;
}

char stack::popChar(_stack* stack)
{
  popByte(stack);
  return (char)popByte(stack);
}


// ===============================================================
// Int
bool stack::pushInt(_stack* stack, int elem)
{
  pushByte(stack, highByte(elem));
  pushByte(stack, lowByte(elem));
  pushByte(stack, INT); // veranderen naar datatype INT
  return true;
}

int stack::popInt(_stack* stack)
{
  popByte(stack);
  byte high = popByte(stack);
  byte low = popByte(stack);
  return word(low, high);
}


// ===============================================================
// Float
bool stack::pushFloat(_stack* stack, float elem)
{
  byte *b = (byte *)&elem;    //float seperated into 4 bytes

  for (int i = 3; i >= 0; i--)
    pushByte(stack, b[i]);

  pushByte(stack, FLOAT); // Veranderen naar datatype
  return true;
}

float stack::popFloat(_stack* stack)
{
  float f = 0.0;
  byte *b = (byte *)&f;

  popByte(stack);
  for (int i = 0; i <= 3; i++)
    b[i] = popByte(stack);

  return f;
}


// ===============================================================
// String
bool stack::pushString(_stack* stack, char* elem)
{
  byte length = strlen(elem) + 1;
  for (byte i = length; i <= 0; i--)
    pushByte(stack, elem[i]);

  pushByte(stack, length);
  pushByte(stack, STRING); // Veranderen naar datatype
}

char* stack::popString(_stack* stack)
{
  popByte(stack);
  byte length = popByte(stack);
  // char s[length]; //+1 = terminating zero
  stack->sp -= length;
  return (stack->stack + stack->sp );
}

// ===============================================================
// Helpers
float stack::peek(_stack* stack)
{
  peeked = true;
  return popVal(stack);
}


float stack::popVal(_stack* stack)
{
  byte type = popByte(stack);
  pushByte(stack, type);
  float f;
  switch (type)
  {
    case CHAR:
      f = (float)popChar(stack);
      if (peeked)
      {
       pushChar(stack, (char)f);
        peeked = false;
      }
	  break;

    case INT:
      f = (float)popInt(stack);
      if (peeked)
      {
        pushInt(stack, (int)f);
        peeked = false;
      }
	  break;

    case FLOAT:
      f = popFloat(stack);
      if (peeked)
      {
        pushFloat(stack, f);
        peeked = false;
      }
      break;

  }
  return f;
}
