#include "commands.h"
//#include "files.h"
#include <EEPROM.h>

char inputArray[arrSize];

bool commandRecognized = false;
bool incomingData = false;

EERef noOfFiles = EEPROM[160];



static int commandIndex = sizeof(availableCommands) / sizeof(commands);
static int stubIndex = sizeof(availableCommandStubs) / sizeof(stub);


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  scanBuffer();
  assignCommand();
  //printBuffer();

}

int scanBuffer()
{
  char input;

  while (Serial.available() > 0 && incomingData == false)
  {
    input = Serial.read();

    if (input != '\n')
    {
      inputArray[cnt] = input;
      cnt++;

      if (cnt >= arrSize)
      {
        cnt = arrSize - 1;
      }
      return;
    }

    inputArray[cnt] = '\0';
    cnt = 0;
    incomingData = true;

  }
}


void assignCommand()
{
   if(incomingData)
    {
        for(int i = 0; i < commandIndex; i++)
        {
            if(strcmp(inputArray, availableCommands[i].commandName) == 0)
            {
                void (*funPtr)() = availableCommands[i].funPtr;
                funPtr();
                commandRecognized = true;
                if (inputArray[i] == ' ')
                {
                   //seperate inputbuffer from parameter and argument and call serial.available
                }
                

            }
        }

      if(commandRecognized)
      {
          incomingData = false;
          commandRecognized = false;
          return;
      }
      
      Serial.println((String)"Input '" + inputArray + "'  not recognized! Available commands are: \n");
      printStub();
      incomingData  = false;
    }
}



void printStub()
{
    for(int i = 0; i < stubIndex; i++)
    {
        //printfSerial.printf(availableCommandStubs[i].stubName);
        //Serial.print(availableCommandStubs[i].stubName);
        Serial.print("\n");
    }
   
}


void printBuffer()
{
  if (incomingData == true)
  {
    Serial.print("Your input is: ");
    Serial.println(inputArray);
    incomingData = false;
  }
}

int storeFunc(file *files, char *filename, int size, int data)
{
  int EEPROMStartAddress = 0;
  for (int i = 0; i < 10; i++)
  {
     //byte data = fat[i];
    EEPROM.put(EEPROMStartAddress, fat[i]);
  }
  
    Serial.println("store function");
    return;


}

int retreiveFunc(int address)
{
  byte output;


    Serial.println("retreive function");
}

int eraseFunc(char *fileName)
{
    Serial.println("erase function");
}


void filesFunc()
{
    Serial.println("files function");
}

int freespaceFunc()
{
    int EEPROMLength = EEPROM.length();
    Serial.println("freespace function");
}

int runFunc(char *fileName)
{
    Serial.println("run function");
}

void listFunc()
{
    Serial.println("list function");
}

int suspendFunc(int pID)
{
    Serial.println("suspend function");
}


int resumeFunc(int pID)
{
    Serial.println("resume function");
}

int killFunc(int pID)
{
    Serial.println("kill function");
}
