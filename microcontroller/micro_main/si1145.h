#ifndef _SI1145H_
#define _SI1145H_

#include "sensors.h"

#include "Adafruit_SI1145.h"

Adafruit_SI1145 si = Adafruit_SI1145();

void si1145_setup()
{
  debug_text("INFO: SI1145 sensor setup");
  
  if (!si.begin()) {
    debug_text("ERROR: could not find SI1145 sensor");
  }
}

void sill45_reading(char output[])
{
  char buff[8];
  format_add(output, buff, si.readVisible()); 
  format_add(output, buff, si.readIR());
  // calculated index value not raw
  // format_add(output, buff, si.readUV()/100); // is multiplied by 100as to get integer
}

#endif _SI1145H_
