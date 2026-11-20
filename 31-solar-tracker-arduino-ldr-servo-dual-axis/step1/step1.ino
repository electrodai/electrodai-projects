// LDR test - not compiled yet
void setup() { Serial.begin(9600); }
void loop() {
  Serial.print(analogRead(A0)); Serial.print("\t");
  Serial.print(analogRead(A1)); Serial.print("\t");
  Serial.print(analogRead(A2)); Serial.print("\t");
  Serial.println(analogRead(A3));
  delay(300);
}
