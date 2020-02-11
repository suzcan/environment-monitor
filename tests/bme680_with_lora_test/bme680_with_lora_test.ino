#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

#include <RH_RF95.h>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
// Max size for the driver is the message can send is 251 octates
#define RFM95_MAX 251
 
// Transmission frequency
#define RF95_FREQ 433.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

Adafruit_BME680 bme;

 
#include <Wire.h>
#include <avr/dtostrf.h>
#include <SPI.h>
#include <string.h>


void setup() {
  // put your setup code here, to run once:

  if(!bme.begin()){
    Serial.println("ERROR: could not find BME680 sensor");
  } else {
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);
  }

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);
 
  Serial.println("INFO: LoRa feather setup");
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("ERROR: LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("ERROR: setFrequency failed");
    while (1);
  }
  Serial.print("INFO: Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  rf95.setTxPower(23, false);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!bme.performReading()) {
    Serial.println("ERROR: Failed to perform reading from BME680");
  } else {
    Serial.println(bme.temperature); // C
    Serial.println(bme.pressure / 100.0); // hPa
    Serial.println(bme.humidity); // (%)
    Serial.println(bme.gas_resistance / 1000.0); // (KOhms)
    Serial.println(bme.readAltitude(SEALEVELPRESSURE_HPA)); // (m)
  }

  char output[2064] = "Goodbye world";
  rf95.send((uint8_t *) output, 251);
  rf95.waitPacketSent();
  delay(10000);
}
