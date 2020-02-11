#ifndef _SDH_
#define _SDH_

#include "sensors.h"
#include <SD.h>

Sd2Card card;
const int sdCard = 10;

void sd_write(char output[])
{
  delay(10);
  digitalWrite(8, HIGH);
  if(SD.begin(sdCard)) {
    File file = SD.open("snsrtest.csv", FILE_WRITE);
    if (file) {
      file.print(output);
      file.println("");
      file.close();
    } else {
      Serial.println("ERROR: failed to open file");
    }
  } else {
    Serial.println("ERROR: failed to initialize SD card");
  }
  delay(1000);
  digitalWrite(8, LOW);
}

#endif
