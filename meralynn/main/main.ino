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
// storage.initFAT();
// storage.deleteFile("test");
// storage.addFile("test", sizeof(test), test);
// storage.addFile("prog1", sizeof(prog1), prog1);
// storage.addFile("prog2", sizeof(prog2), prog2);
// storage.addFile("prog3", sizeof(prog3), prog3);
// storage.addFile("prog4", sizeof(prog4), prog4);
// storage.addFile("prog5", sizeof(prog5), prog5);
// storage.addFile("prog6", sizeof(prog6), prog6);
// storage.deleteFile("prog4");
// storage.deleteFile("prog5");
}


void loop()
{
	terminal.execTerminal();
    terminal.execPrograms();
}
