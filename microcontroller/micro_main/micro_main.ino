#include "bme680.h"
#include "lora.h"
#include "scd30.h"

char output[128] = "Reading: ";

void setup()
{
  Serial.begin(115200);
  Serial.println("begin");
  lora_setup();
  bme680_setup();
}

void loop()
{
  bme680_reading(output);
  scd30_reading(output);
  Serial.println(output);
  memset(output, 0, sizeof output);
}
