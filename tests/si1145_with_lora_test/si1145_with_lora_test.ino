#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

#include <Wire.h>
#include "Adafruit_SI1145.h"
#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
 
// Transmission frequency
#define RF95_FREQ 433.0
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     4
#include "SparkFun_SCD30_Arduino_Library.h"
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp; 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include <avr/dtostrf.h>
#include <SPI.h>
#include <string.h>


#define MIC A0
#define A1 A1
#define A2 A2
#define A3 A3
#define A4 A4
#define A5 A5

const int sampleWindow = 10000;


char output[2064] = "";

SCD30 airSensor;

Adafruit_SI1145 uv = Adafruit_SI1145();
RH_RF95 rf95(RFM95_CS, RFM95_INT);
Adafruit_BME680 bme;



#define pmsSerial Serial1


struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct pms5003data data;


boolean readPMSdata(Stream *s) {
  if (!s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }

  if(false) {
    for (uint8_t i=2; i<32; i++) {
      Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.println(", ");
    }
  }
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    Serial.println("ERROR: Checksum failure for PMS5003 reading");
    return false;
  }
  return true;
}

void format_add(char output[], char buff[], double val) {
  dtostrf(val, 6, 2, buff);
  strcat(output, buff);
  strcat(output, ",");
  memset(buff, 0, sizeof(buff));
}



uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

void draw_text(String s) {
  display.clearDisplay();

  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(10,0);              // Start at top-left corner
  display.println(s);
  display.display();
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(1);
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("ERROR: SSD1306 allocation failed"));
    for(;;); 
  }
  display.display(); 

  if (!sgp.begin()){
    Serial.println("ERROR: could not find SGP30 sensor");
  }

  if(!bme.begin()){
    Serial.println("ERROR: could not find BME680 sensor");
   } else {
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);
  }
  Serial.println("Adafruit SI1145 test");
  
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
    while (1);
  }

  Serial.println("OK!");

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  delay(100);
  Serial.println("Feather LoRa TX Test!");
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

  if(!airSensor.begin()){
    Serial.println("ERROR: could not find SCD30 sensor");
  }
}

void loop() {
   char buff[8];
  analogReadResolution(10);
  unsigned int sample;
  unsigned long startMillis = millis();
  unsigned int signalMax = 0;

  while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(MIC);
      if (sample < 1024)  // toss out spurious readings
         if (sample > signalMax)
            signalMax = sample;
   }

  format_add(output, buff, signalMax);
  analogReadResolution(12);
  unsigned int num = 0;
  unsigned long startMillis2 = millis();
  
  unsigned int total1 = 0;
  unsigned int total2 = 0;
  unsigned int total3 = 0;
  unsigned int total4 = 0;
  unsigned int total5 = 0;

  while(millis() - startMillis2 < sampleWindow)
  {
    total1 += analogRead(A1);
    total2 += analogRead(A2);
    total3 += analogRead(A3);
    total4 += analogRead(A4);
    total5 += analogRead(A5);
    num++;
  }
  
  format_add(output, buff, total1/num);
  format_add(output, buff, total2/num);
  format_add(output, buff, total3/num);
  format_add(output, buff, total4/num);
  format_add(output, buff, total5/num);
  
  Serial.println("SI READING");
  Serial.println("===================");
  format_add(output, buff, uv.readVisible());
  format_add(output, buff, uv.readIR());
  
  // Uncomment if you have an IR LED attached to LED pin!
  //Serial.print("Prox: "); Serial.println(uv.readProx());

  float UVindex = uv.readUV();
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  UVindex /= 100.0;  
  format_add(output, buff, UVindex);
  Serial.println("BME READING");
  if(!bme.performReading()) {
    Serial.println("ERROR: Failed to perform reading from BME680");
  } else {
    Serial.println(bme.temperature); // C
    Serial.println(bme.pressure / 100.0); // hPa
    Serial.println(bme.humidity); // (%)
    Serial.println(bme.gas_resistance / 1000.0); // (KOhms)
    Serial.println(bme.readAltitude(SEALEVELPRESSURE_HPA)); // (m)
  }

  Serial.println("SCD30 READING");
  if (airSensor.dataAvailable())
  {
    Serial.print("co2(ppm):");
    format_add(output, buff, airSensor.getCO2());

    Serial.print(" temp(C):");
    format_add(output, buff, airSensor.getTemperature());

    Serial.print(" humidity(%):");
    format_add(output, buff, airSensor.getHumidity());
  }
  else
    Serial.println("No data");


   float temperature = 24.0; //bme.temperature; // [°C]
  float humidity = 50.0; //bme.humidity; // [%RH]
  sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));

  Serial.println("SGP READING");
  if (!sgp.IAQmeasure() || !sgp.IAQmeasureRaw()) {
    Serial.println("ERROR: Failed to perform reading from SGP30");
  } else {
    // calculations and not raw values
    format_add(output, buff, sgp.TVOC); // ppb\t
    format_add(output, buff, sgp.rawH2); // \t
    format_add(output, buff, sgp.rawEthanol); // 
  }

  Serial.println("DISPLAY DISPLAYING");
  draw_text("Hello World");

  Serial.println("PMS5003 READING");
  while(!readPMSdata(&pmsSerial)){}
  // concentration units standard
  format_add(output, buff, data.pm10_standard); // pm 1.0
  format_add(output, buff, data.pm25_standard); // pm 2.5
  format_add(output, buff, data.pm100_standard); // pm 10
  // concentration units environmental
  format_add(output, buff, data.pm10_env); // pm 1.0
  format_add(output, buff, data.pm25_env); // pm 2.5
  format_add(output, buff, data.pm100_env); // pm 10
  // data particles
  format_add(output, buff, data.particles_03um); // particles > 0.3um / 0.1L air
  format_add(output, buff, data.particles_05um); // particles > 0.5um / 0.1L air
  format_add(output, buff, data.particles_10um); // particles > 1.0um / 0.1L air
  format_add(output, buff, data.particles_25um); // particles > 2.5um / 0.1L air
  format_add(output, buff, data.particles_50um); // particles > 5.0um / 0.1L air
  format_add(output, buff, data.particles_100um); // particles > 10.0um / 0.1L air
  data = pms5003data();

  Serial.println("LORA TRANSMISSION");
 // char output[2064] = "Goodbye world";
 Serial.println(output);
  rf95.send((uint8_t *) output, 251);
  rf95.waitPacketSent();
  delay(3000);

}
