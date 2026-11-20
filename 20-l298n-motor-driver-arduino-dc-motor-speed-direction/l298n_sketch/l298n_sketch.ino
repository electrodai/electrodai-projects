// ElectroDai - L298N: DC motor speed and direction
// ENA -> D5 (PWM), IN1 -> D7, IN2 -> D8

const int ENA = 5;
const int IN1 = 7;
const int IN2 = 8;

void forward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);     // 0 to 255
}

void backward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
}

void stopMotor() {
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  Serial.begin(9600);
  stopMotor();
}

void loop() {
  Serial.println("Forward, speeding up");
  for (int s = 100; s <= 255; s += 5) {   // many small motors need ~100 to start
    forward(s);
    delay(40);
  }
  delay(1000);

  Serial.println("Stop");
  stopMotor();
  delay(1000);                // pause before reversing protects the driver and gears

  Serial.println("Backward, half speed");
  backward(128);
  delay(2000);

  stopMotor();
  delay(2000);
}
