#include "FAT.h"


//==============================================================================
// Initialize File Allocation Table

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
    file emptyFile = (file) {
      "", 0, 0
    };
    writeFATEntry(i, emptyFile);
  }

  Serial.println("initFAT: completed");
  return true;
}

//==============================================================================


// Check if filename already exists in FAT
int fat::existsInFAT(char* filename)
{
  for (int i = 0; i < FAT_SIZE; i++)
  {
   file eempromfile = readFATEntry(i);
    if (strcmp(eepromfile.name, name) == 0) return i;
  }
  return -1;
}


// Find first empty file?
int fat::firstEmptyFile()
{
  for (byte i = 0; i < FAT_SIZE; i++)
  {
    file eepromfile = readFATEntry(i);
    if (eepromfile.length == 0) return i;
  }
  return -1;
}

//==============================================================================
// Read functions

fat::file fat::readFATEntry(byte pos)
{
  file f;
  EEPROM.get(pos, f);
  return f;
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

//==============================================================================
// Write functions


// NOT COMPLETE/WORKING PROPERLY
void fat::writeFATEntry(byte pos, file f)
{
  EEPROM.put(pos, f);
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


//Get starting position of next file in fat?? -- dont understand
int fat::getNextFileStartPos(int i)
{
  for (byte n = i + 1; n < FAT_SIZE; n++)
  {
    file eepromfile = readFATEntry(n);
    if (eepromfile.length != 0) return eepromfile.beginPos;
  }
  return (EEPROM.length() + 1);

}

// Get first availablle byte to store data
int fat::getStartPos(int size)
{
  // noOfFiles + FAT size + padding byte
  int firstFreePos = sizeof(noOfFiles) + (sizeof(file) * FAT_SIZE) + 1;
  
  if (noOfFiles == 0)
    return firstFreePos;
    
  else {
    bool firstFile = true;
    for (byte i = 0; i < FAT_SIZE; i++)
    {
      file eepromfile = readFATEntry(i);
      
      if (eepromfile.length > 0)
      {
        // Check if file fits between FAT and first file
        if (firstFile == true && size < eepromfile.beginPos - firstFreePos)
          return firstFreePos;

        // Check if file fits between current file and next file or end
        if (size < getNextFileStartPos(i) - (eepromfile.beginPos + eepromfile.length))
          return (eepromfile.beginPos + eepromfile.length) + 1;

        firstFile = false;
      }
    }


  }
  return -1;
}

//==============================================================================
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
  int entry = nameToFATEntry(name);
  if (entry >= 0)
  {
    file eepromfile = readFATEntry(entry);
    return readData(eepromfile.beginPos, eepromfile.size);
  }
  else
    return "no file found";
}

//==============================================================================

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


//From Lex van Teeffelen
bool fat::listFiles()
{
  for (byte i = 0; i < FAT_SIZE; i++)
  {
    file eepromfile = readFATEntry(i);
    if (file.size > 0) // Only print files that contain data
    {
      Serial.print("file: ");
      Serial.print(i);                                  // File index from FAT
      Serial.print("\t- ");
      Serial.print(eepromfile.name);                    // Filename
      Serial.print("\t\t");
      Serial.print(eepromfile.beginsPos);               // Start byte on disk
      Serial.print("/");
      Serial.print(eepromfile.beginPos + file.length);  // End byte on disk
      Serial.print("\t(");
      Serial.print(eepromfile.length);                  // Filesize
      Serial.println(" bytes)");
    }
  }
  return true;
}
