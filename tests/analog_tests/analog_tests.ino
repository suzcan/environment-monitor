int a1 = 6;
int a2 = 7;
int a3 = 8;
int a4 = 9;
int a5 = 10;

void setup() {
  Serial.begin(9600);
}

void loop() {
  analogReadResolution(12);
  
  Serial.print("A1: "); Serial.println(analogRead(a1));
  Serial.print("A2: "); Serial.println(analogRead(a2));
  Serial.print("A3: "); Serial.println(analogRead(a3));
  Serial.print("A4: "); Serial.println(analogRead(a4));
  Serial.print("A5: "); Serial.println(analogRead(a5));
}
