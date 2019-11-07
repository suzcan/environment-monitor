#ifndef _SCD30H_
#define _SCD30H_

#include "sensors.h"

#include "SparkFun_SCD30_Arduino_Library.h" 

SCD30 scd;

void scd30_setup()
{
  Wire.begin();
  if(!scd.begin()){
    debug_text("ERROR: could not find SCD30 sensor");
  }
}

void scd30_reading(char output[])
{
  // data available every 2 seconds
  if(!scd.dataAvailable()) {
    debug_text("ERROR: Failed to perform reading from SCD30");
    return;
  }

  char buff[8];
  format_add(output, buff, scd.getCO2()); // ppm
  format_add(output, buff, scd.getTemperature()); // C
  format_add(output, buff, scd.getHumidity()); // (%)
}
#endif
