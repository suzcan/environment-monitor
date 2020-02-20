#include <SD.h>
 
File myFile;
int chipSelect = 10;
 
void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  // pinMode(10, OUTPUT);

  digitalWrite(8, HIGH);
  while(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    chipSelect++;
  }
  Serial.println("initialization done.");
 
  // open the file for reading:
  myFile = SD.open("snsrdata.csv");
  if (myFile) {
    Serial.println("snsrdata.csv");
 
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening snsrtest.csv");
  }
}
 
void loop()
{
  // nothing happens after setup
}
