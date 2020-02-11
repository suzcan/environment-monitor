#ifndef _SI1145H_
#define _SI1145H_

#include "sensors.h"

#include "Adafruit_SI1145.h"

Adafruit_SI1145 si = Adafruit_SI1145();

void si1145_setup()
{
  Serial.println("INFO: SI1145 sensor setup");
  Wire.begin();
  if (!si.begin()) {
    Serial.println("ERROR: could not find SI1145 sensor");
  }
}

void sill45_reading(char output[])
{
  char buff[8];
  format_add(output, buff, si.readVisible()); 
  format_add(output, buff, si.readIR());
  // calculated index value not raw
  float UVindex = si.readUV();
  // is multiplied by 100, as to get integer value
  UVindex /= 100.0;  
  format_add(output, buff, UVindex); 
  si.reset();
  delay(5000);
}

#endif
