#ifndef _SCD30H_
#define _SCD30H_

#include "sensors.h"

void scd30_setup() {
  if(!airSensor.begin()){
    Serial.println("ERROR: could not find SCD30 sensor");
  }
}

void scd30_reading(char output[]){
  if (airSensor.dataAvailable()) {
    format_add(output, buff, airSensor.getCO2()); // co2(ppm)
    format_add(output, buff, airSensor.getTemperature()); // temp(C)
    format_add(output, buff, airSensor.getHumidity()); // humidity(%)
  }
  else{
    Serial.println("ERROR: SCD30 NO DATA");
    format_add(output, buff, FAILED)); // co2(ppm)
    format_add(output, buff, FAILED); // temp(C)
    format_add(output, buff, FAILED); // humidity(%)
  }
}

#endif
