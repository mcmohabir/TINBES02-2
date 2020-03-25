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

  private:
    
    typedef struct {
      char name[12];
      int beginPos;
      int length;
    } file;

    file readFATEntry(byte pos);
    void writeFATEntry(byte pos, file f);
};






#endif
