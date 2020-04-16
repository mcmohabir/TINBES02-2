#ifndef FILEALLOCATIONSYSTEM
#define FILEALLOCATIONSYSTEM

#include <EEPROM.h>
#include "Arduino.h"

#define MAX_FAT_ENTRIES 10
#define MAX_FILE_DESC 12
#define EEPROM_START_ADDR 1

class fileAllocationSystem
{

public:
    static EERef noOfFiles;//start storing the inputted files in location 160 of EEPROM

    static bool initializeFileAllocationTable();
    static int fileInTable();
    static bool existsInTable(char* fileName);
    static bool addFileToTable(char* fileName, int size, char* data);
    static bool deleteFile(char* fileName);
    static bool printFilenames();
    static char* readFileName(char* fileName);
    static int freespace();

private:
    static int firstEmptyFile();
    static bool writeFileData(int startPosition, int size, char* data);
    static char* readFileData(int position, int size);
    static int getFileStart(int size);

    typedef struct fileStructure
    {
       char fileName[MAX_FILE_DESC];
       int startPosition;
       int length;
    } file;

  static file readFatEntry(byte position);
  static bool writeFatEntry(byte position, file f);
};


#endif   
