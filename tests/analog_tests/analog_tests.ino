int a1 = 6;
int a2 = 7;
int a3 = 8;
int a4 = 9;
int a5 = 10;

#define A1 6
#define A2 7
#define A3 8
#define A4 9
#define A5 10

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  //analogReadResolution(12);

  /*
  Serial.print("A1: "); Serial.println(analogRead(a1));
  Serial.print("A2: "); Serial.println(analogRead(a2));
  Serial.print("A3: "); Serial.println(analogRead(a3));
  Serial.print("A4: "); Serial.println(analogRead(a4));
  Serial.print("A5: "); Serial.println(analogRead(a5));
  */

  Serial.print("A1: "); Serial.println(analogRead(A1));
  Serial.print("A2: "); Serial.println(analogRead(A2));
  Serial.print("A3: "); Serial.println(analogRead(A3));
  Serial.print("A4: "); Serial.println(analogRead(A4));
  Serial.print("A5: "); Serial.println(analogRead(A5));
}
