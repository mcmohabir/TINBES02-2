#include "instruction.h"

bool instruction::execute(int id)
{
	Serial.print(F("Execute: "));
	Serial.println(id);
}
