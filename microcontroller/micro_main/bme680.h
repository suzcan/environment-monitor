#ifndef _BME680H_
#define _BME680H_

#include "sensors.h"

#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;

void bme680_setup() {
  debug_text("INFO: BME680 sensor setup");

  if(!bme.begin()){
    debug_text("ERROR: could not find BME680 sensor");
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
    debug_text("ERROR: Failed to perform reading from BME680");
    return;
  }

  char buff[8];
  format_add(output, buff, bme.temperature); // C
  format_add(output, buff, bme.pressure / 100.0); // hPa
  format_add(output, buff, bme.humidity); // (%)
  format_add(output, buff, bme.gas_resistance / 1000.0); // (KOhms)
  format_add(output, buff, bme.readAltitude(SEALEVELPRESSURE_HPA)); // (m)
}
#endif
