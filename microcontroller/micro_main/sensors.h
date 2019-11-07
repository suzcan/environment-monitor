#ifndef _SENSORSH_
#define _SENSORSH_

/*
 * Bosch BME680 // done
 * Sensirion SCD30 // tested
 * Sensirion SGP30 // tested
 * Plantower 5003 // tested
 * Silicon Lab Si1145 // tested 
 * MAX4466 Microphone in A0 //tested
 * Alphasense 3 Channel AFE in A[1-3] //tested
 */
 
#include <Wire.h>
#include <avr/dtostrf.h>
#include <SPI.h>
#include <string.h>

 // helper functions

void format_add(char output[], char buff[], double val) {
  dtostrf(val, 6, 2, buff);
  strcat(output, buff);
  strcat(output, ", ");
  memset(buff, 0, sizeof(buff));
}

#endif
