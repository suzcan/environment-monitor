#include "bme680.h"
#include "lora.h"
#include "scd30.h"
#include "sgp30.h"
#include "pms5003.h"
#include "si1145.h"
#include "analog.h"
#include "mic.h"

char output[1024] = "Reading: ";

void setup()
{
  Serial.begin(115200);
  Serial.println("begin");
  lora_setup();
  bme680_setup();
  scd30_setup();
  sgp30_setup();
  pms5003_setup();
  si1145_setup();
  analog_setup();
}

void loop()
{
  bme680_reading(output);
  scd30_reading(output);
  sgp30_reading(output);
  pms5003_reading(output);
  sill45_reading(output);
  analog_reading(output);
  mic_reading(output);
  lora_transmit(output);
  Serial.println(output);
  memset(output, 0, sizeof output);
}
