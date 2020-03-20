#include <SD.h>
 
File myFile;
int chipSelect = 10;
int cs = 0;
 
void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  digitalWrite(8, HIGH);
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(cs, OUTPUT);
 
  while(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    chipSelect++;
  }
  Serial.println("initialization done.");
 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  SD.remove("test.txt");
  //SD.remove("snsrtest.csv");
  //snsrdata.csv
  myFile = SD.open("ahdbdata.csv", FILE_WRITE);
  delay(1000);
  // if the file opened okay, write to it:
  while(!myFile) {
    myFile = SD.open("ahdbdata.csv", FILE_WRITE);
    Serial.print("Checking: ");Serial.println(cs);
  }
  Serial.print("Success with: ");Serial.println(cs);
  if (myFile) {
    Serial.print("Writing to file...");
    // 26 data points
    myFile.print("time, devID, analog1, analog2, analog3, analog4, analog5, ");
    myFile.print("bme-temperature (C), bme-pressure (hPa), bme-humidity (%), bme-gas resistance (KOhms), bme-altitude (m), ");
    myFile.print("pms-pm 1.0 standard, pms-pm 2.5 standard, pms-pm 10 standard, pms-pm 1.0 environmental, pms-pm 2.5 environmental, pms-pm 10 environmental, pms-particles (0.3um/0.1L), pms-particles (0.5um/0.1L), pms-particles (1.0um/0.1L), pms-particles (2.5um/0.1L), pms-particles (5.0um/0.1L), pms-particles (10.0um/0.1L), ");
    myFile.print("scd-C02 (ppm), scd-temperature (C), scd-humidity (%), ");
    myFile.print("sgp-TVOC (ppb/t), sgp-H2 (/t), sgp-Ethanol, ");
    myFile.print("si-Visible, si-Infrared, si-UV Index");
    //myFile.print("TEST ACCESS");
    myFile.println("");
  // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
  
  digitalWrite(8, LOW);
}
 
void loop()
{
  // nothing happens after setup
}
