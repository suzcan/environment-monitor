#ifndef _SDH_
#define _SDH_

#include "sensors.h"
#include <SD.h>

const int sdCard = 9;

void sd_write(char output[])
{
  digitalWrite(8, HIGH);
  pinMode(9, OUTPUT);
  if(SD.begin(sdCard)) {
    File file = SD.open("snsrtest.csv", FILE_WRITE);
    if (file) {
      file.print(output);
      file.close();
    } else {
      Serial.println("ERROR: failed to open file");
    }
  } else {
    Serial.println("ERROR: failed to initialize SD card");
  }
  digitalWrite(8, LOW);
}

#endif
