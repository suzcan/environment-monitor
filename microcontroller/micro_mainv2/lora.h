#ifndef _LORAH_
#define _LORAH_

#include "sensors.h"

#include <RH_RF95.h>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
// Max size for the driver is the message can send is 251 octates
#define RFM95_MAX 251
 
// Transmission frequency
#define RF95_FREQ 433.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
void lora_setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);
 
  Serial.println("INFO: LoRa feather setup");
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("ERROR: LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("ERROR: setFrequency failed");
    while (1);
  }
  Serial.print("INFO: Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  rf95.setTxPower(23, false);
}

void lora_transmit(char output[])
{
  int msg_length = 2064;
  int max_msg_length = RFM95_MAX * 8;
  output[msg_length] = 0;

  rf95.send((uint8_t *) output, 251);
  rf95.waitPacketSent();

  delay(1000);
  
}
#endif
