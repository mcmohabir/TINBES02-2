#include"fileAllocationSystem.h"

EERef fileAllocationSystem::noOfFiles = EEPROM[0];



bool fileAllocationSystem::initializeFileAllocationTable()
{
    int firstFatElement = sizeof(noOfFiles) + sizeof(file) * MAX_FAT_ENTRIES;
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
        char* fileEntryName = readFatEntry(i).fileName; //waar haalt ie die filename uit de struct vandaan?
        if (strcmp(fileName, fileEntryName) == 0)
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

    int startPosition = (size);
    if(startPosition == -1)
    {
        return false;
    }

    file fileEntry = (file) {"", startPosition, size};
    strcpy(fileEntry.fileName, fileName); //save filename to file entry
    writeFatEntry(firstEmptyFile(), fileEntry);
    writeFileData(fileEntry.startPosition, size, data);
    noOfFiles = noOfFiles + 1;
    return true;
}


/*privates*/



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


fileAllocationSystem::file fileAllocationSystem::readFatEntry(byte position)
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

bool fileAllocationSystem::writeFatEntry(byte position, file f)
{
    EEPROM.put(EEPROM_START_ADDR + position * sizeof(file), f);
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



int fileAllocationSystem::getFileStart(int size)
{
    static int firstFatElement = sizeof(noOfFiles) + sizeof(file) * MAX_FAT_ENTRIES;
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
            fileAllocationSystem::file fileOnFs = readFatEntry(i);

            if(fileOnFs.length > 0)
            {
                if(firstFileInFs == true && size < fileOnFs.startPosition - firstEmptySpot)
                {
                    return firstEmptySpot;
                }

                if(size < getFileStart(i) - (fileOnFs.startPosition) - firstEmptySpot)
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
        return readFileData(fileOnFs.startPosition, fileOnFs.length);
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
        fileOnFs.fileName[0] = '\0';
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



bool fileAllocationSystem::printFilenames()
{
    for(byte i = 0; i < MAX_FAT_ENTRIES; i++)
    {
        file fileEntry = readFatEntry(i);
        if(fileEntry.length > 0)
        {
            Serial.print("File attributes: ");
            Serial.print(i);
            Serial.println("File name: ");
            Serial.print(fileEntry.fileName);
            Serial.println("File start position: ");
            Serial.print(fileEntry.startPosition);
            Serial.println("File end position: ");
            Serial.print(fileEntry.startPosition + fileEntry.length); //TODO: veranderen naar fileEntry.length
            Serial.print("File size: ");
            Serial.print(fileEntry.length);
            Serial.println(" B");
        }
    }
}

int fileAllocationSystem::freespace()
{
    int availableSpace = EEPROM.length();
    availableSpace = availableSpace - (sizeof(file) * MAX_FAT_ENTRIES); //calculate the total size that fs can take
    if(noOfFiles == 10)
    {
        return -1;
    }

    for(byte i = 0; i < MAX_FAT_ENTRIES; i++)
    {
        file fileEntry = readFatEntry(i);

        if(fileEntry.length > 0)
        {
            availableSpace = availableSpace - fileEntry.length;
        }
    }
    return availableSpace;
}

//int firstFatElement = sizeof(noOfFiles) + sizeof(file) * MAX_FAT_ENTRIES;
