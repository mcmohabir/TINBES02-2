#ifndef FAT_H
#define FAT_H

#pragma once
#include <EEPROM.h>
#include "Arduino.h"

#define FAT_SIZE 10

class fat
{
  public:
    static EERef noOfFiles;

    static bool initFAT();
    static bool addFile(char* name, int size, char* data);
    static bool deleteFile(char* name);
    static char* readFile(char* name);
    static bool listFiles();

  private:

    typedef struct {
      char name[12];
      int beginPos;
      int length;
    } eepromfile;
    
    static eepromfile readFATEntry(byte pos);
    static bool writeFATEntry(byte pos, eepromfile f);
    static int getStartPos(int size);
    static int firstEmptyFile();
    static bool writeData(int startPos, int size, char* data);
    static char* readData(int pos, int size);
    static int getNextFileStartPos(int i);
    static int existsInFAT(char* filename);



};

#endif
