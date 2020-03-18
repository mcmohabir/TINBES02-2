#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <EEPROM.h>

EERef noOfFiles = EEPROM[160];

struct function{
  char name[arrSize];
  int beginPos;
  int length; 
};

struct function FAT[10];

void writeFATEntry();
void readFATEntry();
int fileInFAT();
#endif
