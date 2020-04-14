#ifndef FILEALLOCATIONSYSTEM
#define FILEALLOCATIONSYSTEM

#include <EEPROM.H>
#include "Arduino.h"

#define MAX_FAT_ENTRIES 10

class fileAllocationSystem
{
    public:
    eeRef noOfFiles = EEPROM[160]; //start storing the inputted files in location 160 of EEPROM

    bool initializeFileAllocationTable();
    int fileInTable();
    bool existsInTable(char* fileName);

    private:
    bool addFileToTable(char* fileName, int size, char* data);
    int getStartPosition(int size);
    int firstEmptyFile();
    bool writeFileData(int startPosition, int size, char* data);
    char* readFileData(int position, int size);
    int getNextFileEntry(int file);
    char* readFileName(char* fileName);
    bool deleteFile(char* fileName);
    int firstFatElement = sizeof(noOfFiles) + sizeof(file) * MAX_FAT_ENTRIES;

    typedef  struct fileStructure
    {
       char fileName;
       int startPosition;
       int length;
    }; file

    file readFatEntry(byte position);
    void writeFatEntry(byte position, file f);
};


#endif
