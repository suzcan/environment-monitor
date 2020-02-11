#ifndef _SDH_
#define _SDH_

#include "sensors.h"
#include <SD.h>

Sd2Card card;
const int sdCard = 9;

void sd_write(char output[])
{
  pinMode(9, OUTPUT);
  while(!card.init(SPI_HALF_SPEED, sdCard)){
    Serial.println("ERROR: failed sd card setup");
    digitalWrite(8, HIGH);
  }
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
  digitalWrite(8, LOW);
}

#endif
