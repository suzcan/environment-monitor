// sensors
#include "analog.h"
#include "bme680.h"
#include "pms5003.h"
#include "scd30.h"
#include "sgp30.h"
#include "si1145.h"

// monitor
#include "monitor.h"

// data
#include "lora.h"
#include "sd.h"

char output[2064] = "";


void setup() {
  Serial.begin(9600);
  while(!Serial){ delay(1); }

  Serial.println("Starting setup");
  
  monitor_setup();
  sgp30_setup();
  bme680_setup();
  si1145_setup();
  lora_setup();
  scd30_setup();
  pms5003_setup();
  
  //delay(30000); 
  Serial.println("Finished setup");
}

void loop() {
  //Wire.begin();
  //draw_text("Collecting data...");
  Serial.println("Starting data collection");
  analog_reading(output);
  Serial.println("1");
  mic_reading(output);
  Serial.println("2");
  bme680_reading(output);
  Serial.println("3");
  pms5003_reading(output);
  Serial.println("4");
  scd30_reading(output);
  Serial.println("5");
  sgp30_reading(output);
  Serial.println("6");
  sill45_reading(output);
  Serial.println(output);

  lora_transmit(output);
  Serial.println("finished transmitting");
  //sd_write(output);
  Serial.println("finished writing to sd");
  memset(output, 0, sizeof(output));
  Serial.println("cleared");

  //digitalWrite(SDA, HIGH);
  //digitalWrite(SCL, HIGH);
  
  //draw_text("Done, waiting...");
  delay(5000);
}
