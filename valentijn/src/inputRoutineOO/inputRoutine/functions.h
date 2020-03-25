#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <EEPROM.h>
#include "inputRoutine.h"


EERef noOfFiles = EEPROM[160];

struct function{
  char name[arrSize];
  int beginPos;
  int length; 
};

struct function FAT[10];

int writeFATEntry();
int readFATEntry();
int fileInFAT();
#endif
