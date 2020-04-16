#include "FAT.h"

EERef fat::noOfFiles = EEPROM[0];

//==============================================================================
// Initialize File Allocation Table

bool fat::initFAT()
{
  Serial.println("initFAT: initializing...");

  noOfFiles = 0;
  //  Serial.print("initFAT: noOfFiles: ");
  //  Serial.println(noOfFiles);
  //
  //  Serial.print("initFAT: last FAT byte: ");
  //  Serial.println(sizeof(noOfFiles) + (sizeof(eepromfile) * FAT_SIZE));

  for (byte i = 0; i < FAT_SIZE; i++)
  {
    //    Serial.print("initFAT: write empty file: ");
    //    Serial.println(i);
    eepromfile emptyFile = (eepromfile) {
      "", 0, 0
    };
    writeFATEntry(i, emptyFile);
  }

  Serial.println("initFAT: completed");
  return true;
}

//==============================================================================
// Files

bool fat::addFile(char* name, int size, char* data)
{
  if (noOfFiles >= FAT_SIZE)
  {
    Serial.println("FAT full");
    return false;
  }

  if (existsInFAT(name) != -1)
  {
    Serial.println("Filename exists in FAT");
    return false;
  }

  int startPos = getStartPos(size);
  if (startPos == -1)
  {
    Serial.println("Not enough space available");
    return false;
  }
  eepromfile storeFile = (eepromfile) {
    "",
    startPos,
    size
  };
  strcpy(storeFile.name, name);

  writeFATEntry(firstEmptyFile(), storeFile);
  writeData(storeFile.beginPos, size, data);
  noOfFiles += 1;
  return true;
}


char* fat::readFile(char* name)
{
  int entry = existsInFAT(name);
  if (entry >= 0)
  {
    eepromfile file = readFATEntry(entry);
    return readData(file.beginPos, file.length);
  }
  else
    return "no file found";
}


bool fat::deleteFile(char* name)
{
  int entry = existsInFAT(name);
  if (entry >= 0)
  {
    eepromfile file = readFATEntry(entry);
    file.name[0] = '\0';
    file.beginPos = 0;
    file.length = 0;
    writeFATEntry(entry, file);
    noOfFiles -= 1;
    return true;
  }
  else
    return false;
}

//==============================================================================
// Read and write entry

fat::eepromfile fat::readFATEntry(byte pos)
{
  eepromfile file;
  EEPROM.get(START + pos * sizeof(eepromfile), file);
  return file;
}


bool fat::writeFATEntry(byte pos, eepromfile file)
{
  EEPROM.put(START + pos * sizeof(eepromfile), file);
  return true;
}

//==============================================================================
// Read and write data
char* fat::readData(int pos, int size)
{
  char* data = new char[size]; // Variable to store data in
  int nextAdress = 0;
  for (int i = 0; i < size; i++) // Read all bytes from disk
  {
    data[i] = EEPROM.read(pos + nextAdress);
    nextAdress += sizeof(char);
  }
  return data;
}


bool fat::writeData(int startPos, int size, char* data)
{
  int nextAdress = 0;
  for (int i = 0; i < size; i++) // Store all bytes to disk
  {
    EEPROM.write(startPos + nextAdress, data[i]);
    nextAdress += sizeof(data[i]);
  }
  return true;
}


//==============================================================================
//Get starting positions helper functions

int fat::getNextFileStartPos(int i)
{
  for (byte n = i + 1; n < FAT_SIZE; n++)
  {
    eepromfile file = readFATEntry(n);
    if (file.length != 0) return file.beginPos;
  }
  return (EEPROM.length() + 1);

}

// Get first available byte to store data
int fat::getStartPos(int size)
{
  // noOfFiles + FAT size + padding byte
  int firstFreePos = sizeof(noOfFiles) + (sizeof(eepromfile) * FAT_SIZE) + 1;

  if (noOfFiles == 0)
    return firstFreePos;

  else {
    bool firstFile = true;
    for (byte i = 0; i < FAT_SIZE; i++)
    {
      eepromfile file = readFATEntry(i);

      if (file.length > 0)
      {
        // Check if file fits between FAT and first file
        if (firstFile == true && size < file.beginPos - firstFreePos)
          return firstFreePos;

        // Check if file fits between current file and next file or end
        if (size < getNextFileStartPos(i) - (file.beginPos + file.length))
          return (file.beginPos + file.length) + 1;

        firstFile = false;
      }
    }


  }
  return -1;
}


//==============================================================================
// Helper functions
int fat::freespace()
{
  // Returns total free space, not largest free space between files
  if (noOfFiles == 10) return -1;

  int freeSpace = EEPROM.length();
  freeSpace -= sizeof(noOfFiles);
  freeSpace -= (sizeof(eepromfile) * FAT_SIZE);

  int unusedSpace = freeSpace - ((MAX_ARG_SIZE) * FAT_SIZE);
  freeSpace -= unusedSpace;
  for (byte i = 0; i < FAT_SIZE; i++)
  {
    eepromfile file = readFATEntry(i);
    if (file.length > 0)
      freeSpace -= file.length;
  }

  return freeSpace;

}


int fat::existsInFAT(char* filename)
{
  for (byte i = 0; i < FAT_SIZE; i++)
  {
    eepromfile file = readFATEntry(i);
    if (strcmp(file.name, filename) == 0) return i;
  }
  return -1;
}

int fat::firstEmptyFile()
{
  for (byte i = 0; i < FAT_SIZE; i++)
  {
    eepromfile file = readFATEntry(i);
    if (file.length == 0) return i;
  }
  return -1;
}


bool fat::listFiles()
{
  bool fileExists = false;

  for (byte i = 0; i < FAT_SIZE; i++)
  {
    eepromfile file = readFATEntry(i);
    if (file.length > 0) // Only print files that contain data
    {
      fileExists = true;
      Serial.print("file: ");
      Serial.print(i);                                  // File index from FAT
      Serial.print("\t- ");
      Serial.print(file.name);                    // Filename
      Serial.print("\t\t");
      Serial.print(file.beginPos);               // Start byte on disk
      Serial.print("/");
      Serial.print(file.beginPos + file.length);  // End byte on disk
      Serial.print("\t(");
      Serial.print(file.length);                  // Filesize
      Serial.println(" bytes)");
    }
  }

  if (!fileExists)
    Serial.println("No files in FAT");

  return true;
}
