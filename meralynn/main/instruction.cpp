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
            Serial.print(F("unknown unary operation: "));
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
	float ytype = stack.popByte(&(process->stack));
	stack.pushByte(&(process->stack), ytype);
	float y = stack.popVal(&(process->stack));

	//Peek type
	float xtype = stack.popByte(&(process->stack));
	stack.pushByte(&(process->stack), xtype);
	float x = stack.popVal(&(process->stack));

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
			z = (int) x % (int) y;
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
		case(LESSTHANOREQUALS):
			z = (x <= y ? 1 : 0);
			ztype = CHAR;
			break;
		case(GREATERTHAN):
			z = (x > y ? 1 : 0);
			ztype = CHAR;
			break;
		case(GREATERTHANOREQUALS):
			z = (x >= y ? 1 : 0);
			ztype = CHAR;
			break;
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
			z = (x && y ? 1 : 0);
			ztype = CHAR;
			break;
		case(LOGICALOR):
			z = (x || y ? 1 : 0);
			ztype = CHAR;
			break;
		case(LOGICALXOR):
			z = (!x != !y ? 1 : 0);
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
		stack.pushChar(&(process->stack), (char) z);
	else if (ztype == INT)
		stack.pushInt(&(process->stack), (int) z);
	else if (ztype == FLOAT)
		stack.pushFloat(&(process->stack), z);
}

void instruction::branchOp(process::proc *process, byte operation)
{

		switch (operation) {
			case (LOOP):
				process->loopAddr = process->procCtr;
				break;
			case (ENDLOOP):
				process->procCtr = process->loopAddr;
				break;
			case (IF):
			{
				float value = stack.peek(&(process->stack));
				if (value == 0)
					process->procCtr += EEPROM[process->procCtr]+1; // procCtr al eerder verhoogd;
																	// ziet next value
				break;
			}
			case (ELSE):
			{
				float value = stack.peek(&(process->stack));
				if (value != 0)
					process->procCtr += EEPROM[process->procCtr]+1; // procCtr al eerder verhoogd;
																	// ziet next value
				break;
			}
			case (ENDIF):
				stack.popVal(&(process->stack));
				break;
			case (WHILE):
			{
				float value = stack.peek(&(process->stack));
				if (value == 0)
					process->procCtr += EEPROM[process->procCtr+1] +2;
				else{
					stack.pushByte(&(process->stack), 4 + (EEPROM[process->procCtr]) + EEPROM[process->procCtr + 1]);
					process->procCtr += 2;
				}
				break;
			}
			case (ENDWHILE):
			{
				byte value = stack.popByte(&(process->stack));
				process->procCtr -= value;
				break;
			}
			default:
				Serial.print(F("unknown bran. operation"));
				Serial.println(operation, DEC);

		}
}

void instruction::arduinoOp(process::proc *process, byte operation)
{
	switch (operation) // Check operation
   {
	   case (CONSTRAIN):
	   {
		   // Pop in reverse order
		   int max = (int) stack.popVal(&(process->stack)); // Maximum
		   int min = (int) stack.popVal(&(process->stack)); // Minimum
		   int x = (int) stack.popVal(&(process->stack)); // Constrain value
		   x = min(max(x, min), max); // Constrain x
		   stack.pushInt(&(process->stack), x); // Push result
		   break;
	   }
	   case (MAP):
	   {
		   // Pop in reverse order
		   int out_max = (int) stack.popVal(&(process->stack));
		   int out_min = (int) stack.popVal(&(process->stack));
		   int in_max = (int) stack.popVal(&(process->stack));
		   int in_min = (int) stack.popVal(&(process->stack));
		   int x = (int) stack.popVal(&(process->stack)); // Value to map
		   x = map(x, in_min, in_max, out_min, out_max); // Map x
		   stack.pushInt(&(process->stack), x); // Push result
		   break;
	   }
	   case (PINMODE):
	   case (DIGITALWRITE):
	   case (ANALOGWRITE):
	   {
		   // Pop in reverse order
		   int value = (int) stack.popVal(&(process->stack));
		   int pin = (int) stack.popVal(&(process->stack));
		   if (operation == PINMODE)
			   pinMode(pin, value); // Set pinmode
		   else if (operation == ANALOGWRITE)
			   analogWrite(pin, value); // Set analog pin value
		   else if (operation == DIGITALWRITE)
			   digitalWrite(pin, value); // Set digital pin value
		   break;
	   }
	   default:
		   Serial.print(F("unknown arduino operation: "));
		   Serial.println(operation, DEC);
   }


}
void instruction::timeOp(process::proc *process, byte operation)
{
	/*
	*	Int overflow: van unsigned long naar een int
	*	Program spazzes
	*/
	switch (operation) {
		case (DELAY):
			delay((int) stack.popVal(&(process->stack)));
		case (DELAYUNTIL):
		{
			// Get delay from stack
			int x = (int) stack.popVal(&(process->stack));
			if(x > (int) millis())
			{
				stack.pushInt(&(process->stack), x);
				process->procCtr -= 1; // Delay not done, so lower PC
			}
			break;
		}
		case (MILLIS):
			stack.pushInt(&(process->stack), (int) millis());
			break;
		default:
			Serial.print(F("unknown time operation: "));
			Serial.println(operation, DEC);
	}
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
