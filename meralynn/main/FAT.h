#ifndef FAT_H
#define FAT_H

#include <EEPROM.h>
#include "Arduino.h"

#define FAT_SIZE 10

class fat
{
  public:
    EERef noOfFiles = EEPROM[160]; //store in position 160 in EEPROM
    void writeFATEntry();
    void readFATEntry();
    int fileInFAT();

    bool existsInFAT(char* filename);

  private:

    struct file {
      char name[arrSize];
      int beginPos;
      int length;
    };
};






#endif
