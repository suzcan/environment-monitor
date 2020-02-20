#include "sensors.h"
#include "analog.h"
#include "bme680.h"
#include "display.h"
#include "lora.h"
#include "pms5003.h"
#include "rtc.h"
#include "scd30.h"
#include "sd.h"
#include "sgp30.h"
#include "si1145.h"

char output[2064] = "";

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup");

  rtc_setup();
  display_setup();
  sgp30_setup();
  bme680_setup();
  si1145_setup();
  lora_setup();
  scd30_setup();
  pms5003_setup();
}

void loop() {
  draw_text("Collecting data...");
  Serial.println("Starting data collection");
  rtc_reading(output);
  // DEVICE ID
  format_add(output, buff, 1);
  analog_reading(output);
  bme680_reading(output);
  pms5003_reading(output);
  scd30_reading(output);
  sgp30_reading(output);
  si1145_reading(output);
  Serial.println(output);

  lora_transmit(output);
  sd_write(output);
  memset(output, 0, sizeof(output));
  
  draw_text("Done, waiting...");
  delay(50000);
}
