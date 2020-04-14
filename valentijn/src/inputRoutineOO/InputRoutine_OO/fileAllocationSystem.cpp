#include"fileAllocationSystem.h"

bool fileAllocationSystem::initializeFileAllocationTable
{
    noOfFiles = 0; //start with 0 files
    Serial.print("FAT initialized, number of files that currently reside within the filesystem ");
    Serial.println(noOfFiles);
    Serial.print("Last element of FAT: ");
    Serial.println(firstFatElement);

    for (byte i = 0; i < MAX_FAT_ENTRIES; i++)
    {
        /*fill all ten fat slots with new empty file structs*/
        Serial.print("Write new file: ");
        Serial.println(i);
        file newEmptyFile = (file){"",0,0};
        writeFatEntry(i, newEmptyFile);
    }

    Serial.println("New empty files writing complete!");
    return true;
}

bool fileAllocationSystem::existsInTable(char* fileName)
{
    for (int i = 0; i < MAX_FAT_ENTRIES; i++)
    {
        char* FileEntryName = ReadFatEntry(i).fileName; //waar haalt ie die filename uit de struct vandaan?
        if (strcmp(fileName, FileEntryName) == 0);
        {
            return true;
        }
    }
    return false;
}

bool fileAllocationSystem::addFileToTable(char* fileName, int size, char* data)
{
    if(!existsInTable(fileName))
    {/*if file does not exist within file allocation table, return false*/
        return false;
    }

    int startPosition = getStartPosition(size);
    if(startPosition == -1)
    {
        return false;
    }

    file fileEntry = (file) {"", startPosition, size};
    strcpy(fileEntry.name, fileName); //save filename to file entry
    writeFatEntry(firstEmptyFile(), fileEntry);
    writeFileData(fileEntry.startPosition, size, data);
    noOfFiles = noOfFiles + 1;
    return true;
}


int fileAllocationSystem::firstEmptyFile()
{
    for (byte i = 0; i < MAX_FAT_ENTRIES; i++)
    {
        file fileOnFs = readFatEntry(i);

        if(fileOnFs.length == 0)
        {
            return i;
        }
        return -1;
    }
}


void fileAllocationSystem::readFatEntry(byte position)
{
    file fileOnFs;
    EEPROM.get(position, fileOnFs);

    return fileOnFs;
}



bool fileAllocationSystem::writeFileData(int startPosition, int size, char* data)
{
    int nextFileAddress = 0;
    for(int i = 0; i < size; i++)
    {   /*Write each file at start position until last byte, then increment the next address to write a new file*/
        EEPROM.write(startPosition + nextFileAddress, data[i]);
        nextFileAddress = nextFileAddress + sizeof(data[i]);
    }

    return true;
}


char* fileAllocationSystem::readFileData(int position, int size)
{
    int nextFileAddress = 0;
    char* fileData = new char[size];

    for(int i = 0; i < size; i++)
    {
        fileData[i] = EEPROM.read(position + nextFileAddress);
        nextFileAddress = nextFileAddress + sizeof(char);
    }

    return fileData;
}



int fileAllocationSystem::getNextFileEntry(int file)
{
    int firstEmptySpot = firstFatElement + 1;
    bool firstFileInFs = false;

    if(noOfFiles == 0)
    {
        return firstEmptySpot;
    }
    else
    {
        firstFileInFs = true;
        for(byte i = 0; i < MAX_FAT_ENTRIES; i++)
        {
            file fileOnFs = readFatEntry(i);
            if(fileOnFs.length > 0)
            {
                if(firstFileInFs == true && size < fileOnFs.startPosition - firstEmptySpot)
                {
                    return firstEmptySpot;
                }

                if(size < getNextFileEntry(i) - (fileOnFs.startPosition) - firstEmptySpot)
                {
                    return (fileOnFs.startPosition + fileOnFs.length) + 1;
                }
            }
        }

        firstFileInFs = false;
    }

    return -1;
}


char* fileAllocationSystem::readFileName(char* fileName)
{/*TODO: implement file entry counter function*/
    int fileEntry = existsInTable(fileName);
    if(fileEntry >= 0)
    {
        file fileOnFs = readFatEntry(fileEntry);
        return readFileData(fileOnFs.startPosition, fileOnFs.size);
    }
    else
    {
        Serial.print("Error, file not found!");
    }
}

bool fileAllocationSystem::deleteFile(char* fileName)
{
    int fileEntry = existsInTable(fileName);
    if(fileEntry > 0)
    {
        file fileOnFs = readFatEntry(fileEntry);
        fileOnFs.name[0] = '\0';
        fileOnFs.startPosition = 0;
        fileOnFs.length = 0;

        writeFatEntry(fileEntry, fileOnFs); //write empty file to new spot
        noOfFiles = noOfFiles - 1;
        return true;
    }
    else
    {
        return false;
    }
}
