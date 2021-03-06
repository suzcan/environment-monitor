const int sampleWindow = 10000; // Sample window width in mS (10000 ms = 10 s)
unsigned int sample;
 
void setup() 
{
   Serial.begin(9600);
   analogReadResolution(10);
}
 
 
void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(A0);
      if (sample < 1024)  // toss out spurious readings
         if (sample > signalMax)
            signalMax = sample;
   }

   Serial.print("Analog read: "); Serial.println(signalMax);
   uint16_t freq = map(signalMax, 0, 1023, 100, 8000);
   Serial.print("Frequency: "); Serial.println(freq);
   
}
