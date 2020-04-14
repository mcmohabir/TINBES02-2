#include "terminal.h"

terminal::commandType terminal::availableCommands[] = {
  {"store", &terminal::store},
  {"retreive", &terminal::retreive},
  {"erase", &terminal::erase},
  {"files", &terminal::files},
  {"freespace", &terminal::freespace},
  {"run", &terminal::run},
  {"list", &terminal::list},
  {"suspend", &terminal::suspend},
  {"resume", &terminal::resume},
  {"kill", &terminal::kill}
};




terminal::terminal()
{
    terminal::reset();
}


void terminal::initializeTerminal()
{
    int commandInput = scanBuffer();
    if (commandInput == 0)
    {
        return;
    }
    
    assignCommand(currentArguments);
}

int terminal::scanBuffer()
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
          currentArgIter = currentArgIter + writeArgument(input);
        }
      return 0;    
    }
    
    incomingData = true;
    printBufferArray();
    return 1;
  }
}

void terminal::assignCommand(char** arguments)
{
     if(!incomingData)
     {
         return;
     }
    
     for(int i = 0; i < commandCounter; i++)
      {
       if(strcmp(currentCommandBuffer, availableCommands[i].commandName) == 0)
        {
           commandFun funPtr = availableCommands[i].funPtr;
          (this->*funPtr)(arguments);
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

bool terminal::writeCommand(char inputChar)
{
     if(inputChar == ' ' || inputChar == '\n')
    {
        return true;
    }

    currentCommandBuffer = chrcat(currentCommandBuffer, inputChar);
    return false;
}

bool terminal::writeArgument(char inputChar)
{
    if(inputChar == ' ' || inputChar == '\n')
    {
        return true;
    }

    currentArguments[currentArgIter] = chrcat(currentArguments[currentArgIter], inputChar);
    return false
}

char* terminal::chrcat(char* appendToChar, char whatTo)
{
    byte length = strlen(appendToChar);

    if (length == (arrSize -1))
    {
        return appendToChar;
    }

    appendToChar[length] = whatTo;
    appendToChar[length + 1] = 0;
    
    return appendToChar; 
}


void terminal::reset()
{
    currentCommandBuffer[0] = '\0';

    for (byte i = 0; i < MAX_ARGS; i++)
    {
        currentArguments[i][0] = '\0'; 
    }
    
    currentArgIter = 0;
    initialCommand = false;
    
}


void terminal::printBufferArray()
{
    if (!incomingData)
    {
        return;
    }

    Serial.print("Command given is: ");
    Serial.println(currentCommandBuffer);

    Serial.print("Arguments given are: ");

    for (int i = 0; i < sizeof(currentArguments) + 1; i++)
    {
        Serial.println(" ");
        Serial.println(currentArguments[i]);
    }
    
    Serial.println(" ");    
}


void terminal::printInfo()
{
    for (int i = 0; i < commandCounter; i++)
    {
        Serial.println(availableCommands[i].name);
        

    }
    Serial.println(" ");   
}

void terminal::retreive(char** arguments)
{
  Serial.println("in retreive function");
}


void terminal::erase(char** arguments)
{
  Serial.println("in erase function");
}


void terminal::files(char** arguments)
{
  Serial.println("in files function");
}


void terminal::freespace(char** arguments)
{
  Serial.println("in freespace function");
}


void terminal::run(char** arguments)
{
  Serial.println("in run function");
}


void terminal::list(char** arguments)
{
  Serial.println("in list function");
}


void terminal::suspend(char** arguments)
{
  Serial.println("in suspend function");
}


void terminal::resume(char** arguments)
{
  Serial.println("in resume function");
}


void terminal::kill(char** arguments)
{
  Serial.println("in kill function");
}


void terminal::store (char** arguments)
{
 
 Serial.println("in store function");
 if (fat::existsInFAT()) 
 {

 }

}
