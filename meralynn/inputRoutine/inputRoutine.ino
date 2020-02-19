
static char bffer[] = 12;
int charCounter = 0;
int input;


void setup() {
  Serial.begin(9600);

}

int readInput() {
 
  if (Serial.available() > 0) {
    input = Serial.read();
    charCounter++;
  }

  if(input!='\n'){
    bffer[charCounter-1] = input;  
    continue;
  }
  
  bffer[charCounter-1] = '\0';
  return 1;
}


void loop() {
  readInput();

}
