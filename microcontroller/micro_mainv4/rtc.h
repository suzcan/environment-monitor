#ifndef _RTCH_
#define _RTCH_

#include "sensors.h"

void rtc_setup() {
  if (! rtc.begin()) {
    Serial.println("ERROR: RTC not found");
  }
  if (!rtc.initialized()) {
    Serial.println("ERROR: RTC is NOT running");
  }
}

void rtc_reading(char output[]){
  DateTime now = rtc.now();
  int currTime = now.unixtime();
  format_add(output, buff, currTime);
}
#endif
