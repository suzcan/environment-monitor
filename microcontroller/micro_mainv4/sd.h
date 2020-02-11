#ifndef _SDH_
#define _SDH_

void sd_write(char output[]) {
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
