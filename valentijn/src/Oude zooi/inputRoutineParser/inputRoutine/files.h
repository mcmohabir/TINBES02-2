#ifndef FILES_H
#define FILES_H



const int fileNameLength = 12;


typedef struct fileStruct
{
    char fileName[fileNameLength];
    int startPosition;
    int length;

} file;


file fat[10];



#endif