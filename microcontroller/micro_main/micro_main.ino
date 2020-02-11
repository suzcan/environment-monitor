#include "sd.h"
#include "bme680.h"
#include "lora.h"
#include "scd30.h"
#include "sgp30.h"
#include "pms5003.h"
#include "si1145.h"
#include "analog.h"
#include "monitor.h"

char output[2064] = "";
char logs[2064] = "";

/*
 * The LoRa module's CS pin is connected to microcontroller pin 8 and pulled low. To use other SPI 
 * devices the pin 8 needs to be pulled high and pulled low for 
 */
void setup()
{
  Serial.begin(115200);
  monitor_setup();
  Serial.println("INFO: Beginning setup");
  //draw_text("INFO: Beginning setup");
  lora_setup();
  bme680_setup();
  pms5003_setup();
  scd30_setup();
  sgp30_setup();
  si1145_setup(); 
  //draw_text("INFO: Finished setup");
}

void loop()
{
  draw_text("Collecting data");
  Serial.println("INFO: Beginning reading output");
  analog_reading(output);
  mic_reading(output);
  bme680_reading(output);
  pms5003_reading(output);
  scd30_reading(output);
  sgp30_reading(output);
  sill45_reading(output);
  //sd_write(output);
  //digitalWrite(8, LOW);
  delay(1000);
  Serial.println(output);
  lora_transmit(output);
  memset(output, 0, sizeof output);
  draw_text("Done!");
  //delay(60000);
}
