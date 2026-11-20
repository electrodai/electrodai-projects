// Sensor test - not compiled yet, check on your board
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
}
void loop() {
  Serial.print("L=");
  Serial.print(digitalRead(2));
  Serial.print("  R=");
  Serial.println(digitalRead(3));
  delay(200);
}
