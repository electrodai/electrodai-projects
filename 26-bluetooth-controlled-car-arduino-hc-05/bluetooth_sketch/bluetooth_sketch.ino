// Bluetooth car with HC-05 - not compiled yet
#include <SoftwareSerial.h>

SoftwareSerial bt(10, 11);   // RX = D10 (from HC-05 TX), TX = D11 (via divider)

const int ENA = 5, IN1 = 7, IN2 = 8;
const int IN3 = 3, IN4 = 4, ENB = 6;

int speedVal = 170;          // 0-255
unsigned long lastCmd = 0;

void drive(int left, int right) {
  digitalWrite(IN1, left  > 0); digitalWrite(IN2, left  < 0);
  digitalWrite(IN3, right > 0); digitalWrite(IN4, right < 0);
  analogWrite(ENA, abs(left));
  analogWrite(ENB, abs(right));
}

void setup() {
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  Serial.begin(9600);
  bt.begin(9600);            // HC-05 default data mode baud rate is commonly 9600
  drive(0, 0);
}

void loop() {
  if (bt.available()) {
    char c = bt.read();
    Serial.println(c);       // for debugging

    if (c >= '0' && c <= '9') {
      speedVal = map(c - '0', 0, 9, 90, 255);
    }

    switch (c) {
      case 'F': drive(speedVal, speedVal);   break;
      case 'B': drive(-speedVal, -speedVal); break;
      case 'L': drive(-speedVal, speedVal);  break;
      case 'R': drive(speedVal, -speedVal);  break;
      case 'S': drive(0, 0);                 break;
      default: break;
    }
    if (c == 'F' || c == 'B' || c == 'L' || c == 'R' || c == 'S') {
      lastCmd = millis();
    }
  }

  // Safety: stop if no command for 1.5 seconds (phone out of range)
  if (millis() - lastCmd >= 1500) {
    drive(0, 0);
  }
}
