#include "terminal.h"
#include "FAT.h"
#include "process.h"
#include "programs.h"

terminal terminal;
fat storage;
void setup()
{
delay(300);
Serial.begin(9600);
Serial.println(F("-- Ready to go! --"));
storage.deleteFile("test");
storage.addFile("test", sizeof(test), test);
// storage.addFile("prog1", sizeof(prog1), prog1);
// storage.addFile("prog2", sizeof(prog2), prog2);
}


void loop()
{
	terminal.execTerminal();
    terminal.execPrograms();
}
