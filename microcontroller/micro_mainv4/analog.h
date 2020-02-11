#ifndef _ANALOGH_
#define _ANALOGH_

#include "sensors.h"

void analog_reading(char output[]) {
  analogReadResolution(10);
  unsigned int sample;
  unsigned long startMillis = millis();
  unsigned int signalMax = 0;

  while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(MIC);
      if (sample < 1024)  // toss out spurious readings
         if (sample > signalMax)
            signalMax = sample;
   }

  format_add(output, buff, signalMax);
  analogReadResolution(12);
  unsigned int num = 0;
  unsigned long startMillis2 = millis();
  
  unsigned int total1 = 0;
  unsigned int total2 = 0;
  unsigned int total3 = 0;
  unsigned int total4 = 0;
  unsigned int total5 = 0;

  while(millis() - startMillis2 < sampleWindow)
  {
    total1 += analogRead(A1);
    total2 += analogRead(A2);
    total3 += analogRead(A3);
    total4 += analogRead(A4);
    total5 += analogRead(A5);
    num++;
  }
  
  format_add(output, buff, total1/num);
  format_add(output, buff, total2/num);
  format_add(output, buff, total3/num);
  format_add(output, buff, total4/num);
  format_add(output, buff, total5/num);
  return;
}

#endif
