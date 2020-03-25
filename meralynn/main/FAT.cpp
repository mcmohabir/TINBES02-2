#include "FAT.h"

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


void fat::writeFATEntry(byte pos, file f)
{
  EEPROM.put(pos, f);
}


file fat::readFATEntry(byte pos)
{
  file f;
  EEPROM.get(pos, f);
  return f;
}
