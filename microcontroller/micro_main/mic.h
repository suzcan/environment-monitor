#ifndef _MICH_
#define _MICH_

#include "sensors.h"

#define MIC 5;

void mic_reading(char output[])
{
  char buff[8];
  format_add(output, buff, analogRead(MIC);
}


#endif _MICH_
