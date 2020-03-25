#include "commands.h"
//#include "files.h"
#include <EEPROM.h>

char inputArray[arrSize];

bool commandRecognized = false;
bool incomingData = false;
bool initialCommand = false;

EERef noOfFiles = EEPROM[160];



static int commandIndex = sizeof(availableCommands) / sizeof(commands);
static int stubIndex = sizeof(availableCommandStubs) / sizeof(stub);


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  //scanBuffer();
  //assignCommand();
  //printBuffer();
  //printBufferArray();
  terminal();

}

void terminal()
{
  scanBuffer();
  assignCommand();
}

int scanBuffer()
{
  char input;

  while (Serial.available() > 0 && incomingData == false)
  {
    input = Serial.read();

    if (input != '\n')
    {
      if(!initialCommand)
      {
        initialCommand = writeCommand(input);
      }
      else
      {
        argsIterator = argsIterator + writeArgument(input);
      }
      return;    
    }
    
    
    incomingData = true;
    printBufferArray();

  }
}


void reset()
{
  currentCommandBuffer[0] = '\0';

  for (byte i = 0; i < MAX_ARGS; i++)
  {
    currentArgs[i][0] = '\0';
  }

  argsIterator = 0;
  initialCommand = false;
}

bool writeCommand(char input)
{
  if (input == ' ' || input == '\n')
  {
    return true;
  }

  currentCommandBuffer = chrcat(currentCommandBuffer, input);
  return false; 
}

bool writeArgument(char input)
{
 if(input == ' ' || input == '\n')
 {
   return true;
 } 

 currentArgs[argsIterator] = chrcat(currentArgs[argsIterator], input);
 return false;
}

char* chrcat (char* appendToChar, char whatTo)
{
  byte len = strlen(appendToChar);

  if (len == (arrSize -1))
  {
    return appendToChar;
  }

 appendToChar[len] = whatTo;
 appendToChar[len + 1] = 0;
 return appendToChar;    
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
                break;
            }
        }

      if(commandRecognized)
      {
          incomingData = false;
          commandRecognized = false;
          reset();
          return;
      }
      Serial.println((String)"Input '" + inputArray + "'  not recognized! Available commands are: \n");
      //printStub();
      printInfo();
      incomingData  = false;
      reset();
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

/*
void printBuffer()
{
  if (incomingData == true)
  {
    Serial.print("Your input is: ");
    Serial.println(inputArray);
    incomingData = false;
  }
}
*/

void printBufferArray()
{
  if(incomingData)
  {
    Serial.print("Given command is: ");
    Serial.println(currentCommandBuffer);

    Serial.print("Given arguments: ");
    for (int i = 0; i < sizeof(currentArgs) + 1; i++)
    {
      Serial.print(" ");
      Serial.print(currentArgs[i]);
    }
    Serial.println();
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


void printInfo()
{
  for (int i = 0; i < commandIndex; i++)
  {
    Serial.println(availableCommands[i].commandName);
  }
  
}