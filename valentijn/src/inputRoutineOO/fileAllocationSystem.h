#ifndef FILEALLOCATIONSYSTEM
#define FILEALLOCATIONSYSTEM

#include <EEPROM.h>
#include "Arduino.h"

#define MAX_FAT_ENTRIES 10

class fileAllocationSystem
{
    public:
    static EERef noOfFiles;//start storing the inputted files in location 160 of EEPROM

    static bool initializeFileAllocationTable();
    static int fileInTable();
    static bool existsInTable(char* fileName);

    private:
    static bool addFileToTable(char* fileName, int size, char* data);
    static int getStartPosition(int size);
    static int firstEmptyFile();
    static bool writeFileData(int startPosition, int size, char* data);
    static char* readFileData(int position, int size);
    static int getNextFileEntry(int file);
    static char* readFileName(char* fileName);
    static bool deleteFile(char* fileName);
    static bool printFilenames();

    typedef struct fileStructure
    {
       char fileName;
       int startPosition;
       int length;
    } file;

  static file readFatEntry(byte position);
  static void writeFatEntry(byte position, file f);
};


#endif
