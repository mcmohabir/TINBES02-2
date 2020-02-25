//declare array of 12  to store serial input
const int arraySize = 12;
char inputArray[arraySize];

//char counter
static int counter = 0;

//semaphore to check if there is incoming data
bool dataIn = false;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    inputHandler();
    printOutput();
}

/*
* @brief: the following function scans the serial bus for input, returns 1 if data is received
* serial data is being stored in inputArray and directly being read with each loop cycle
*/
int inputHandler()
{
    char serialInput;
    
    //check if serial is active and semaphore is false
    while (Serial.available() > 0 && dataIn == false)
    {
        serialInput = Serial.read();
    

        //check for newline 
        if (serialInput != '\n')
        {
            inputArray[counter] = serialInput;
            counter++;

            //if buffer overflows, remove last char
            if(counter >= arraySize)
            {
                counter = arraySize - 1;
            }
            return 1;
        }
        //set stop condition and flag true
        inputArray[counter] = '\0';
        counter = 0;
        dataIn = true;
    }
}

/*
**brief: function that takes the input from the inputArray in runtime and prints it to the console
*/
void printOutput()
{   
    //check if dataIn semaphore is true, print data and set it to false again
    if(dataIn == true)
    {
        Serial.print("Buffer contains the following input: ");
        Serial.println(inputArray);
        dataIn = false;
    }
}