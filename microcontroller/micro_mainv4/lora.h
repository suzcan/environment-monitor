#ifndef _LORAH_
#define _LORAH_

#include "sensors.h"
#include "sd.h"

// handshake variables
String ack = "ACK";
String done = "DN";

void lora_setup(){
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  delay(100);
  Serial.println("Feather LoRa TX Test!");
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("'#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // The default transmitter power is 13dBm, using PA_BOOST.
  rf95.setTxPower(23, false);

  // set address of this box using Id
  rf95.setHeaderId(BOXID);
  rf95.setThisAddress(BOXID);
}

int lora_transmit(char output[]){
  /* No Handshake send output
  rf95.send((uint8_t *) output, 251);
  rf95.waitPacketSent();
  //*/

  // Handshake send output
  int ready_attempt = 0;
  int data_attempt = 0;
  bool success = false;
  char rdy_packet[20] = "RDY";
  rdy_packet[19] = 0;

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  while(ready_attempt < 2) {
    // Sending ready
    rf95.send((uint8_t*)rdy_packet, 20);
    delay(10);
    rf95.waitPacketSent();

    // Wait for acknowledgement
    if(rf95.waitAvailableTimeout(10000)) {
      if(rf95.recv(buf, &len)) {
        String ack_rcv = (char*)buf;
        if(strstr(ack_rcv.c_str(),ack.c_str()) && rf95.headerId() == 99 && rf95.headerTo() == BOXID) {
          while(data_attempt < 2) {
            delay(8000); //  delay for old packet to be unloaded
            rf95.send((uint8_t*) output, 251);
            delay(10);
            rf95.waitPacketSent();
            if(rf95.waitAvailableTimeout(10000)) {
              if(rf95.recv(buf, &len)) {
                String done_rcv = (char*)buf;
                if(strstr(done_rcv.c_str(), done.c_str()) && rf95.headerId() == 99 && rf95.headerTo() == BOXID) {
                  success = true;
                  break;
                }
              }
            } else {
              data_attempt = data_attempt + 1;
            }
          }
          // if attempted to send data 3 times and no response continue
          break;
        }
        delay(5000);
      } else {
        ready_attempt = ready_attempt + 1;
      }
    }
  }
  if (ready_attempt >= 2) {
    return 1;
  }
  if (ready_attempt >= 2) {
    return 2;
  }
  if (!success) {
    return 3;
  }
  return 4;
}

#endif
