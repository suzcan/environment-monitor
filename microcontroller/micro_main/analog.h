#ifndef _ANALOGH_
#define _ANALOGH_

#include "sensors.h"

#define MIC 5
#define A1 6
#define A2 7
#define A3 8
#define A4 9
#define A5 10

void analog_setup()
{
  analogReadResolution(12);
}

void analog_reading(char output[])
{
  char buff[8];
  int a1 = analogRead(A1);
  format_add(output, buff, analogRead(a1));
  //format_add(output, buff, analogRead(A2));
  //format_add(output, buff, analogRead(A3));
  //format_add(output, buff, analogRead(A4));
  //format_add(output, buff, analogRead(A5));
  
}

#endif _ANALOGH_
