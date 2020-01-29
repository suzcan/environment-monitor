#define A1 A1
#define A2 A2
#define A3 A3
#define A4 A4
#define A5 A5

const int sampleWindow = 10000;

void setup() {
  Serial.begin(115200);
}

void loop() {
analogReadResolution(12);
  /*
  Serial.print("A1: "); Serial.println(analogRead(a1));
  Serial.print("A2: "); Serial.println(analogRead(a2));
  Serial.print("A3: "); Serial.println(analogRead(a3));
  Serial.print("A4: "); Serial.println(analogRead(a4));
  Serial.print("A5: "); Serial.println(analogRead(a5));
  */
///*
  
  unsigned int num = 0;
  unsigned long startMillis = millis();
  
  unsigned int total1 = 0;
  unsigned int total2 = 0;
  unsigned int total3 = 0;
  unsigned int total4 = 0;
  unsigned int total5 = 0;

  while(millis() - startMillis < sampleWindow)
  {
    //Serial.println(analogRead(A1));
    //Serial.print("num: "); Serial.println(num);
    
    total1 += analogRead(A1);
    total2 += analogRead(A2);
    total3 += analogRead(A3);
    total4 += analogRead(A4);
    total5 += analogRead(A5);
    num++;
  }

  Serial.print("A1: "); Serial.println(total1/num);
  Serial.print("A2: "); Serial.println(total2/num);
  Serial.print("A3: "); Serial.println(total3/num);
  Serial.print("A4: "); Serial.println(total4/num);
  Serial.print("A5: "); Serial.println(total5/num);
//  */
}
