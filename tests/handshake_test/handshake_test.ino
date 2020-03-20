// Feather9x_TX
// -*- mode: C++ -*-

// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX
 
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
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  Serial.begin(9600);
  // this waits until serial console is open, remove this line if not thethered to computer
  /*while (!Serial) {
    delay(1);
  }
  */
 
  delay(100);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Serial.println("Feather LoRa TX Test!");
 
  // manual reset
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
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
  rf95.setHeaderId(1);
  rf95.setThisAddress(1);
}
 
int16_t packetnum = 0;  // packet counter, we increment per xmission

void draw_text(String s) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(10,0);             // Start at top-left corner
  display.println(s);
  display.display();
}
 
void loop()
{
  bool attempt = true;
  bool resend = true;
  int attempt_no = 0;
  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  Serial.println("Transmitting..."); // Send a message to rf95_server
  // Max length is: 251
  //Serial.print("Max length is: "); Serial.println(rf95.maxMessageLength());

  while(resend && attempt) {
    char radiopacket[20] = "RDY";
    draw_text("Sending..."); // Serial.println(radiopacket);
    radiopacket[19] = 0;
    
    Serial.println("Sending...");
    delay(10);
    rf95.send((uint8_t *)radiopacket, 20);
   
    Serial.println("1 - Waiting for packet to complete..."); 
    delay(10);
    rf95.waitPacketSent();
    // Now wait for a reply
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
   
    Serial.println("1- Waiting for reply...");
    if (rf95.waitAvailableTimeout(5000)) { 
      // Should be a reply message for us now   
      if (rf95.recv(buf, &len)) {
        Serial.println(rf95.headerId()); 
        Serial.println(rf95.headerId() == 99);
        Serial.println(rf95.headerTo());
        int headerId = rf95.headerId();
        int headerTo = rf95.headerTo();
        if(headerId == 99 && headerTo == 1) {
          
          draw_text("Got reply: ");
          draw_text((char*)buf);
          Serial.println((char*)buf);

          delay(100);
          char radiopacket2[20] = "Hello world!";
          draw_text("Sending..."); Serial.println(radiopacket2);
          radiopacket2[19] = 0;
          rf95.send((uint8_t *)radiopacket2, 20);
   
          Serial.println("2 - Waiting for packet to complete..."); 
          delay(1000);
          rf95.waitPacketSent();
          
          if (rf95.waitAvailableTimeout(10000)) { 
            // Should be a reply message for us now   
            if (rf95.recv(buf, &len)){
                Serial.println(rf95.headerId()); 
                headerId = rf95.headerId();
                headerTo = rf95.headerTo();
                if(rf95.headerId() == 99 && rf95.headerTo() == 1) {
                  draw_text("Got reply: ");
                  draw_text((char*)buf);
                  Serial.println((char*)buf);
                }
            } else {
              attempt_no = attempt_no + 1;
            }
          } else {
            draw_text("Receive failed");
          }
          
          } else {
            draw_text("No reply, is there a listener around?");
          }
     }
    }
  }
  delay(1000);
 
}
