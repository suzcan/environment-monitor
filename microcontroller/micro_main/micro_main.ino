char output[128] = "Reading: ";

void setup()
{
  Serial.begin(9600);
  Serial.println("begin");
  lora_setup();
  bme680_setup();
}

void loop()
{
  bme680_reading(output);
  Serial.println(output);
}

