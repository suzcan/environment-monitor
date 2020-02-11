#ifndef _SI1145H_
#define _SI1145H_

#include "sensors.h"

void si1145_setup(){
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
  }
}

void si1145_reading(char output[]){
  format_add(output, buff, uv.readVisible());
  format_add(output, buff, uv.readIR());

  float UVindex = uv.readUV();
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  UVindex /= 100.0;  
  format_add(output, buff, UVindex);
  return;
}

#endif
