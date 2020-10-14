#include "stack.h"
#include "Arduino.h"
byte stack[STACKSIZE];
byte sp = 0;


void pushbyte(byte b) {
  stack[sp++] = b;
}


byte popByte() {
  return stack[sp--];
}
