#include "instruction.h"

bool instruction::execute(int id)
{
	Serial.print(F("Execute: "));
	Serial.println(id);

	// byte nextInstruction  = EEPROM[process::procTable[id].procCtr];
}


//
// bool instruction::suspend(int id)
// {
//
// }
//
// bool instruction::kill(int id)
// {
// 	bool killed = process::killProcess(id);
// 	if(killed)
// 	{
// 		Serial.print(F("Process "));
// 		Serial.print(id);
// 		Serial.println(F(" succesfully terminated"));
// 		return true;
// 	}
// 	Serial.print(F("ERROR: Failed to terminate process"));
// 	Serial.println(id);
// 	return false;
// }
