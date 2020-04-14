#ifndef FAT_H
#define FAT_H

#include <EEPROM.h>
#include "Arduino.h"

#define FAT_SIZE 10

class fat
{
  public:
    EERef noOfFiles = EEPROM[160]; //store in position 160 in EEPROM

    int fileInFAT();
    bool existsInFAT(char* filename);
    bool initFAT();
    char* readFile(char* name);
    bool addFile(char* name, int size, char* data);
    bool deleteFile(char* name);
    
    
  private:

    int getStartPos(int size);
    int firstEmptyFile();
    bool writeData(int startPos, int size, char* data);
    char* readData(int pos, int size);
    int getNextFileStartPos(int i);

    typedef struct {
      char name[12];
      int beginPos;
      int length;
    } file;

    file readFATEntry(byte pos);
    void writeFATEntry(byte pos, file f);
};






#endif
