#ifndef _SENSORSH_
#define _SENSORSH_

// SENSOR LIBRARIES
#include <Wire.h>
#include "Adafruit_SI1145.h"
#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "SparkFun_SCD30_Arduino_Library.h"
#include "Adafruit_SGP30.h"
#include <avr/dtostrf.h>
#include <SPI.h>
#include <string.h>
#include <SD.h>
#include "RTClib.h"

// BME DEFINES
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

// LORA DEFINES
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
// Transmission frequency
#define RF95_FREQ 433.0

// DISPLAY DEFINES
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     4

// ANALOG DEFINES
#define MIC A0
#define A1 A1
#define A2 A2
#define A3 A3
#define A4 A4
#define A5 A5

// PMS5003 DEFINES
#define pmsSerial Serial1

// DEBUG DEFINES
#define DEBUG false
#define FAILED 123456

// SENSOR INSTANTIATIONS
Adafruit_SGP30 sgp; 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SI1145 uv = Adafruit_SI1145();
RH_RF95 rf95(RFM95_CS, RFM95_INT);
Adafruit_BME680 bme;
Sd2Card card;
SCD30 airSensor;
RTC_PCF8523 rtc;

// CONSTANTS
const int sdCard = 10;
const int sampleWindow = 10000;
char buff[8];

// PMS5003 HELPER STRUCTURE
struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
struct pms5003data data;

// FORMAT HELPER FUNCTION
void format_add(char output[], char buff[], double val) {
  dtostrf(val, 6, 2, buff);
  strcat(output, buff);
  strcat(output, ",");
  memset(buff, 0, sizeof(buff));
}

#endif
