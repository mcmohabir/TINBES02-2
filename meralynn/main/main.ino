#include "proc.h"

proc procm;

void setup()
{
	delay(300);
  Serial.begin(9600);
  procm.run();

}


void loop()
{
}
