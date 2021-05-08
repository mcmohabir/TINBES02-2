#include "instruction.h"
#include "instruction_set.h"
#include "stack.h"
#include <EEPROM.h>
stack stack;

void instruction::valToStack(process::proc* process, byte datatype)
{
		if(datatype == STRING)
		{
			uint_least8_t length = 0;
			char nextChar = 0;
			do {
				nextChar = EEPROM[process->procCtr++];
				length++;
				stack.pushByte(&(process->stack), nextChar);
			} while(nextChar != 0);

			stack.pushByte(&(process->stack), length);
		}
		else
		{
			for(uint_least8_t i = 0; i < datatype; i++)
				stack.pushByte(&(process->stack), EEPROM[process->procCtr++]);
		}
		stack.pushByte(&(process->stack), datatype);
}

void instruction::unaryOp(process::proc* process, byte operation)
{
	// Serial.print("unop: ");
    // Peek type so we can push it correctly after a operation
    byte type = stack.popByte(&(process->stack));
	stack.pushByte(&(process->stack), type);
	// Serial.println(type);
    // Get the data as float from stack
    float value = stack.popVal(&(process->stack));
	// Serial.println(value);

    switch (operation) // Perform operation on data as float
    {
        case (INCREMENT):
            value += 1.0f;
            break;
        case (DECREMENT):
            value -= 1.0f;
            break;
        case (UNARYMINUS):
            value *= -1.0f;
            break;
        case (ABS):
            value = (value >= 0 ? value : value*-1.0f);
            break;
        case (SQ):
            value = value * value;
            break;
        case (SQRT):
            value = sqrt(value);
            break;
        case (ANALOGREAD):
            value = analogRead(value);
            type = INT; // Push as INT
            break;
        case (DIGITALREAD):
            value = digitalRead(value);
            type = CHAR; // Push as CHAR
            break;
        case (LOGICALNOT):
            value = (value == 0.0f ? 1 : 0);
            type = CHAR; // Push as CHAR
            break;
        case (BITWISENOT):
            Serial.println(F("BITWISENOT"));
            break;
        case (TOCHAR):
            value = round(value);
            type = CHAR; // Push as CHAR
            break;
        case (TOINT):
        case (ROUND): // Does the same as TOINT
            value = round(value) + 0;
            type = INT; // Push as INT
            break;
        case (TOFLOAT):
            type = FLOAT; // Push as FLOAT
            break;
        case (FLOOR):
            value = floor(value);
            type = INT; // Push as INT
            break;
        case (CEIL):
            value = ceil(value);
            type = INT; // Push as INT
            break;
        default:
            Serial.print(F("unknown operation: "));
            Serial.println(operation, DEC);
            break;
    }

    // Push (float) data back on the stack by type
    if (type == CHAR)
        stack.pushChar(&(process->stack), (char) value);
    else if (type == INT)
        stack.pushInt(&(process->stack), (int) value);
    else if (type == FLOAT)
        stack.pushFloat(&(process->stack), value);
}

void instruction::print(process::proc* process, bool newline=false)
{
    byte type = stack.popByte(&(process->stack)); // Peek datatype
	stack.pushByte(&(process->stack), type);
    switch(type) // Print by popping the datatype
    {
        case (CHAR):
            Serial.print(stack.popChar(&(process->stack)));
            break;
        case (STRING):
            Serial.print(stack.popString(&(process->stack)));
            break;
        case (INT):
            Serial.print(stack.popInt(&(process->stack)), DEC);
            break;
        case (FLOAT):
            Serial.print(stack.popFloat(&(process->stack)), 3);
            break;
    }

    if (newline)
        Serial.println(); // Add a newline if PRINTLN was used
}
