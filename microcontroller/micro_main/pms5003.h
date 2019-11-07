#ifndef _PMS5003H_
#define _PMS5003H_

#include "sensors.h"

#define pmsSerial Serial1

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

void pms5003_setup()
{
  debug_text("INFO: PMS5003 sensor setup")
  if(!pmsSerial.begin(115200)){
    debug_text("ERROR: could not find PMS5003 sensor");
  }
}


void pms5003_readings(char output[])
{
  if(!readPMSdata(&pmsSerial)){
    debug_text("ERROR: Failed to perform reading from PMS5003");
    return;
  } else {
    char buff[8];
    // concentration units standard
    format_add(output, buff, data.pm10_standard); // pm 1.0
    format_add(output, buff, data.pm25_standard); // pm 2.5
    format_add(output, buff, data.pm100_standard); // pm 10
    // concentration units environmental
    format_add(output, buff, data.pm10_env); // pm 1.0
    format_add(output, buff, data.pm25_env); // pm 2.5
    format_add(output, buff, data.pm100_env); // pm 10
    // data particles
    format_add(output, buff, data.particles_05um); // particles > 0.3um / 0.1L air
    format_add(output, buff, data.particles_05um); // particles > 0.5um / 0.1L air
    format_add(output, buff, data.particles_10um); // particles > 1.0um / 0.1L air
    format_add(output, buff, data.particles_25um); // particles > 2.5um / 0.1L air
    format_add(output, buff, data.particles_50um); // particles > 5.0um / 0.1L air
    format_add(output, buff, data.particles_100um); // particles > 10.0um / 0.1L air
  }
}

boolean readPMSdata(Stream *s) {
  if (!s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }
 
  for (uint8_t i=2; i<32; i++) {
    debug_text("0x"); debug_text(buffer[i], HEX); debug_text(", ");
  }
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    debug_text("ERROR: Checksum failure for PMS5003 reading");
    return false;
  }
  return true;
}


#endif _PMS5003H_
