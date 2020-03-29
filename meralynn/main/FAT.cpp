#include "FAT.h"

bool fat::initFAT()
{
    Serial.println("initFAT: initializing...");

    noOfFiles = 0;
    Serial.print("initFAT: noOfFiles: ");
    Serial.println(noOfFiles);

    Serial.print("initFAT: last FAT byte: ");
    Serial.println(sizeof(noOfFiles) + (sizeof(file) * FAT_SIZE));

    for (byte i = 0; i < FAT_SIZE; i++)
    {
        Serial.print("initFAT: write empty file: ");
        Serial.println(i);
        file emptyFile = (file){"",0,0};
        writeFATEntry(i, emptyFile);
    }

    Serial.println("initFAT: completed");
    return true;
}


bool fat::existsInFAT(char* filename)
{
  for (int i = 0; i < FAT_SIZE; i++)
  {
    char* entryName = readFATEntry(i).name;
    if (strcmp(filename, entryName) == 0)
    {
      return true;
    }
  }
  return false;
}



int fat::firstEmptyFile()
{
  for (byte i = 0; i < FAT_SIZE; i++)
  {
    file eepromfile = readFATEntry(i);
    if (eepromfile.length == 0) return i;
  }
  return -1;
}


void fat::writeFATEntry(byte pos, file f)
{
  EEPROM.put(pos, f);
}


fat::file fat::readFATEntry(byte pos)
{
  file f;
  EEPROM.get(pos, f);
  return f;
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


int fat::getNextFileStartPos(int i)
{
  for (byte n = i + 1; n < FAT_SIZE; n++)
  {
    file eepromfile = readFATEntry(n);
    if (eepromfile.length != 0) return eepromfile.beginPos;
  }
  return (EEPROM.length() + 1);

}


int fat::getStartPos(int size)
{
  int firstFreePos = sizeof(noOfFiles) + (sizeof(file) * FAT_SIZE) + 1;
  if (noOfFiles == 0)
    return firstFreePos;  
  else {
    bool firstFile = true;
    for (byte i = 0; i < FAT_SIZE; i++)
    {
      file  eepromfile = readFATEntry(i);
      if (eepromfile.length > 0)
      {
        if (firstFile == true && size < eepromfile.beginPos - firstFreePos)
          return firstFreePos;

        if (size < getNextFileStartPos(i) - (eepromfile.beginPos + eepromfile.length))
          return (eepromfile.beginPos + eepromfile.length) + 1;

        firstFile = false;
      }
    }


  }
  return -1;
}


bool fat::addFile(char* name, int size, char* data)
{
  if (!existsInFAT(name))
  {
    return false;
  }

  int startPos = getStartPos(size);
  if (startPos == -1)
    return false;

  file storeFile = (file) {
    "", startPos, size
  };
  strcpy(storeFile.name, name);
  writeFATEntry(firstEmptyFile(), storeFile);
  writeData(storeFile.beginPos, size, data);
  noOfFiles += 1;
  return true;
}

char* fat::readFile(char* name)
{
  int entry = nameToFATEntry(name);
  if(entry >= 0)
  {
    file eepromfile = readFATEntry(entry);
    return readData(eepromfile.beginPos, eepromfile.size);
  }
  else
    return "no file found";
}

bool fat::deleteFile(char* name)
{
  int entry = existsInFAT(name);
  if (entry >= 0)
  {
    file eepromfile = readFATEntry(entry);
    eepromfile.name[0] = '\0';
    eepromfile.beginPs = 0;
    eepromfile.length = 0;
    writeFATEntry(entry, eepromfile);
    noOfFiles -= 1;
    return true;
  }
  else
    return false;
}
