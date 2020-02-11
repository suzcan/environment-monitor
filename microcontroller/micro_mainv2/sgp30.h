#ifndef _SGP30H_
#define _SGP30H_

#include "sensors.h"

#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;

uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

void sgp30_setup()
{
  if (!sgp.begin()){
    Serial.println("ERROR: could not find SGP30 sensor");
  }
}

void sgp30_reading(char output[])
{
  float temperature = 24.0; //bme.temperature; // [°C]
  float humidity = 50.0; //bme.humidity; // [%RH]
  sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));
  char buff[8];

  if (!sgp.IAQmeasure() || !sgp.IAQmeasureRaw()) {
    Serial.println("ERROR: Failed to perform reading from SGP30");
    format_add(output, buff, FAILED); // ppb\t
    // format_add(output, buff, sgp.eCO2); // ppm
    format_add(output, buff, FAILED); // \t
    format_add(output, buff, FAILED); // 
    return;
  } else {
    // calculations and not raw values
    format_add(output, buff, sgp.TVOC); // ppb\t
    // format_add(output, buff, sgp.eCO2); // ppm
    // raw measurements
    format_add(output, buff, sgp.rawH2); // \t
    format_add(output, buff, sgp.rawEthanol); // 
  }
}

#endif
