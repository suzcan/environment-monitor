#ifndef _BME680H_
#define _BME680H_

#include "sensors.h"

void bme680_setup() {
  if(!bme.begin()){
    Serial.println("ERROR: could not find BME680 sensor");
   } else {
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);
  }
}

void bme680_reading(char output[]){
  if(!bme.performReading()) {
    Serial.println("ERROR: Failed to perform reading from BME680");
    format_add(output, buff, FAILED); // C
    format_add(output, buff, FAILED); // hPa
    format_add(output, buff, FAILED); // (%)
    format_add(output, buff, FAILED); // (KOhms)
    format_add(output, buff, FAILED); // (m)
  } else {
    format_add(output, buff, bme.temperature); // C
    format_add(output, buff, bme.pressure / 100.0); // hPa
    format_add(output, buff, bme.humidity); // (%)
    format_add(output, buff, bme.gas_resistance / 1000.0); // (KOhms)
    format_add(output, buff, bme.readAltitude(SEALEVELPRESSURE_HPA)); // (m)
  }
  return;
}
#endif
