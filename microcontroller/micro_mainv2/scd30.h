#ifndef _SCD30H_
#define _SCD30H_

#include "sensors.h"

#include "SparkFun_SCD30_Arduino_Library.h" 

SCD30 scd;

void scd30_setup()
{
  if(!scd.begin()){
    Serial.println("ERROR: could not find SCD30 sensor");
  }
}

void scd30_reading(char output[])
{
  char buff[8];
  // data available every 2 seconds
  if(!scd.dataAvailable()) {
    Serial.println("ERROR: Failed to perform reading from SCD30");
    format_add(output, buff, FAILED); // ppm
    format_add(output, buff, FAILED); // C
    format_add(output, buff, FAILED); // (%)
    return;
  } else {
    format_add(output, buff, scd.getCO2()); // ppm
    format_add(output, buff, scd.getTemperature()); // C  
    format_add(output, buff, scd.getHumidity()); // (%)
  }
  
}
#endif
