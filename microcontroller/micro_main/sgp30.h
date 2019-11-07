#ifndef _SGP30H_
#define _SGP30H_

#include "sensors.h"

#include "Adafruit_SGP30.h

Adafruit_SGP30 sgp;

void sgp30_setup()
{
  if (!sgp.begin()){
    debug_text("ERROR: could not find SGP30 sensor");
  }
}

void sgp30_reading(char output[])
{
  float temperature = 22.1; // [°C]
  float humidity = 45.2; // [%RH]
  sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));

  if (!sgp.IAQmeasure() || !sgp.IAQmeasureRaw()) {
    debug_text("ERROR: Failed to perform reading from SGP30");
    return;
  }

  char buff[8];
  // calculations and not raw values
  // format_add(output, buff, sgp.TVOC); // ppb\t
  // format_add(output, buff, sgp.eCO2); // ppm

  // raw measurements
  format_add(output, buff, sgp.rawH2); // \t
  format_add(output, buff, sgp.rawEthanol); // 
}

#endif
