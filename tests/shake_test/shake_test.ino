#include <stdio.h>
#include <string.h>

#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
 
// Transmission frequency
#define RF95_FREQ 433.0
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RH_RF95 rf95(RFM95_CS, RFM95_INT);

#define BOX_ID 1

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  Serial.begin(9600);
  /*
  while (!Serial) {
    delay(1);
  }
  */
 
  delay(100);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Serial.println("Feather LoRa TX Test!");
 
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);
  rf95.setHeaderId(BOX_ID);
  rf95.setThisAddress(BOX_ID);
}

String ack = "ACK";
String done = "DN";

void loop() {
  Serial.println("Transmitting...");
  int ready_attempt = 0;
  int data_attempt = 0;
  bool success = false;

  while(ready_attempt < 2) {
    // Sending ready
    char radiopacket[20] = "RDY";
    radiopacket[19] = 0;
    rf95.send((uint8_t*)radiopacket, 20);
    Serial.println("Sending...");
    delay(10);
    rf95.waitPacketSent();

    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    Serial.println("Waiting for acknowledgement");
    if (rf95.waitAvailableTimeout(10000)) {
      if (rf95.recv(buf, &len)) {
        Serial.println("Received a packet with:");
        String ack_rcv = (char*)buf;
        if(strstr(ack_rcv.c_str(),ack.c_str()) && rf95.headerId() == 99 && rf95.headerTo() == BOX_ID) {
          while(data_attempt < 2) {
            Serial.println("Recognized ACK, sending DATA");
            char dif_packet[251] = "DEVICE 1";
            dif_packet[250] = 0;
            delay(8000);
            rf95.send((uint8_t*)dif_packet, 20);
            delay(10);
            rf95.waitPacketSent();
            if (rf95.waitAvailableTimeout(10000)) {
             if (rf95.recv(buf, &len)) {
              String done_rcv = (char*)buf;
              if(strstr(done_rcv.c_str(), done.c_str()) && rf95.headerId() == 99 && rf95.headerTo() == BOX_ID) {
                success = true;
                break;
              }
             }
            } else {
              data_attempt = data_attempt + 1;
            }
          }
          break;
        }
        delay(5000);
      }
    } else {
      ready_attempt = ready_attempt + 1;
    }
  }
  if(ready_attempt >= 2) {
    Serial.println("Tried to send message 3 time no response"); 
  }
  if(data_attempt >= 2) {
    Serial.println("Tried to send data 3 times no response");
  }
  Serial.print("Was succesfull: "); Serial.println(success);
  delay(15000);
} 
