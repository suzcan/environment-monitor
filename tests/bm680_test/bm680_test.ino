/***************************************************************************
  This is a library for the BME680 gas, humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/3660

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <avr/dtostrf.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)
#define BME680_DEBUG

Adafruit_BME680 bme;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("BME680 test");


  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  Serial.println("setting up");
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  Serial.println("finished setting up");
}

void loop() {
  Serial.println("starting loop");
 
  char output[128] = "Reading: ";
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  /*
  Serial.println("starting reading");
  char buff[8];
  strcat(output, "Temperature (C) =");
  dtostrf(bme.temperature, 6, 2, buff);
  strcat(output, buff);
  memset(buff, 0, sizeof(buff));
  
  strcat(output, "Pressure (hPa) =");
  dtostrf(bme.pressure / 100.0, 6, 2, buff);
  strcat(output, buff);
  memset(buff, 0, sizeof(buff));

  
  strcat(output, "Humidity (%) =");
  dtostrf(bme.humidity, 6, 2, buff);
  strcat(output, buff);
  memset(buff, 0, sizeof(buff));
  
  strcat(output, "Gas (KOhms) =");
  dtostrf(bme.gas_resistance / 1000.0, 6, 2, buff);
  strcat(output, buff);
  memset(buff, 0, sizeof(buff));
  
  strcat(output, "Approx. Altitude (m) =");
  dtostrf(bme.readAltitude(SEALEVELPRESSURE_HPA), 6, 2, buff);
  strcat(output, buff);
  memset(buff, 0, sizeof(buff));

  Serial.println(output);
  Serial.println("finished");
  /*/
  //*
  
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  
  Serial.println("finished loop");
  delay(2000);
}
