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

    // Peek type so we can push it correctly after a operation
    byte type = stack.popByte(&(process->stack));
	stack.pushByte(&(process->stack), type);

    // Get the data as float from stack
    float value = stack.popVal(&(process->stack));

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
            type = INT;
            break;
        case (DIGITALREAD):
            value = digitalRead(value);
            type = CHAR;
            break;
        case (LOGICALNOT):
            value = (value == 0.0f ? 1 : 0);
            type = CHAR;
            break;
        case (BITWISENOT):
            Serial.println(F("BITWISENOT"));
            break;
        case (TOCHAR):
            value = round(value);
            type = CHAR;
            break;
        case (TOINT):
        case (ROUND):
            value = round(value) + 0;
            type = INT;
            break;
        case (TOFLOAT):
            type = FLOAT;
            break;
        case (FLOOR):
            value = floor(value);
            type = INT;
            break;
        case (CEIL):
            value = ceil(value);
            type = INT;
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


void instruction::binaryOp(process::proc* process, byte operation)
{
	//Pop Y first
	//Peek type
	float ytype = stack::popByte(&(process->stack));
	stack::pushByte(&(process->stack), ytype);
	float y = stack::popVal(&(process->stack));

	//Peek type
	float xtype = stack::popByte(&(process->stack));
	stack::pushByte(&(process->stack), xtype);
	float x = stack::popVal(&(process->stack));

	// Use Z as output value, type determined by the largest type of X or Y
    float ztype = (xtype > ytype ? xtype : ytype); // Get largest type
    float z;

	switch (operation)
	{
		case (PLUS):
			z = x + y;
			break;
		case (MINUS):
			z = x - y;
			break;
		case (TIMES):
			z = x * y;
			break;
		case (DIVIDEDBY):
			z = x / y;
			break;
		case (MODULUS):
			z = x % y;
			break;
		case(EQUALS):
			z = (x == y ? 1 : 0);
			ztype = CHAR;
			break;
		case(NOTEQUALS):
			z = (x != y ? 1: 0);
			ztype = CHAR; // voor test: toInt()
			break;
		case(LESSTHAN):
			z = (x < y ? 1 : 0);
			ztype = CHAR;
			break;
		case(LESTHANOREQUALS):
			z = (x <= y ? 1 : 0);
			ztype = CHAR;
			break;
		case(GREATERTHAN):
			z = (x > y ? 1 : 0);
			ztype = CHAR;
			break:
		case(GREATERTHANOREQUALS):
			z = (x >= y ? 1 : 0);
			ztype = CHAR;
			break:
		case (MIN):
			z = (x < y ? x : y);
			break;
		case (MAX):
			z = (x > y ? x : y);
			break;
		case (POW):
			for (int i = 0; i < y; i++)
				z *= x;
			break;
		case (LOGICALAND):
			z = (x && y ? 1 : 0)
			ztype = CHAR;
			break;
		case(LOGICALOR):
			z = (x || y ? 1 : 0)
			ztype = CHAR;
			break;
		case(LOGICALXOR):
			z = (x ^^ y ? 1 : 0);
			ztype = CHAR;
			break;
		case (BITWISEAND):
			z = (int) x & (int) y;
			break;
		case (BITWISEOR):
			z = (int) x | (int) y;
			break;
		case (BITWISEXOR):
			z = (int) x ^ (int) y;
			break;
		default:
			Serial.print(F("unknown binary operation: "));
			Serial.println(operation, DEC);
	}

	// Push output value (float z) back on the stack by ztype
	if (ztype == CHAR)
		stack::pushChar(&(process->stack), (char) z);
	else if (ztype == INT)
		stack::pushInt(&(process->stack), (int) z);
	else if (ztype == FLOAT)
		stack::pushFloat(&(process->stack), z);
}


void instruction::timeOp(process::proc *process, int operation)
{

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
