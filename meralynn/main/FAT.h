#ifndef FAT_H
#define FAT_H

#pragma once
#include <EEPROM.h>
#include "Arduino.h"

#define FAT_SIZE 10
#define START 1
#define MAX_COMMAND_SIZE 12
#define MAX_ARG_SIZE 60
class fat
{
  public:
    static EERef noOfFiles;
    static bool initFAT();
    static char* readFile(char* name);
    static bool addFile(char* name, int size, char* data);
    static bool deleteFile(char* name);
    static bool listFiles();
    static int freespace();
  private:

    typedef struct {
      char name[14];
      int beginPos;
      int length;
    } eepromfile;

    static eepromfile readFATEntry(byte pos);
    static bool writeFATEntry(byte pos, eepromfile f);
    static char* readData(int pos, int size);
    static bool writeData(int startPos, int size, char* data);
    static int firstEmptyFile();
    static int getNextFileStartPos(int i);
    static int getStartPos(int size);
    static int existsInFAT(char* filename);



};

#endif