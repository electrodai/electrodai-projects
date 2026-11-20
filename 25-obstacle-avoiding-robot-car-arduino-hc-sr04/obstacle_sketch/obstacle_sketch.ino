// Obstacle avoiding car - not compiled yet
#include <Servo.h>

const int TRIG = 2, ECHO = 3, SERVO_PIN = 11;
const int ENA = 5, IN1 = 7, IN2 = 8;
const int IN3 = 9, IN4 = 10, ENB = 6;

const int SAFE_CM  = 25;   // stop if closer than this
const int SPEED    = 140;  // 0-255
const int TURN_MS  = 450;  // tune: time to turn about 90 degrees

Servo eye;

long readCm() {
  digitalWrite(TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long us = pulseIn(ECHO, HIGH, 25000UL);
  delay(60);                            // let echoes clear before the next ping
  if (us == 0 || us > 23200UL) return 401; // no usable echo; treat as beyond range
  return us / 58;
}

// median of 3 readings to remove spikes
long distance() {
  long a = readCm(), b = readCm(), c = readCm();
  if ((a >= b && a <= c) || (a <= b && a >= c)) return a;
  if ((b >= a && b <= c) || (b <= a && b >= c)) return b;
  return c;
}

void drive(int left, int right) {
  digitalWrite(IN1, left  > 0); digitalWrite(IN2, left  < 0);
  digitalWrite(IN3, right > 0); digitalWrite(IN4, right < 0);
  analogWrite(ENA, abs(left));
  analogWrite(ENB, abs(right));
}

void setup() {
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  eye.attach(SERVO_PIN);
  eye.write(90);
  delay(500);
}

void loop() {
  eye.write(90);
  long front = distance();

  if (front > SAFE_CM) {
    drive(SPEED, SPEED);                 // clear: forward
  } else {
    drive(0, 0);                         // stop
    delay(200);
    drive(-SPEED, -SPEED);               // back up a little
    delay(300);
    drive(0, 0);

    eye.write(30);  delay(500);          // look right
    long right = distance();
    eye.write(150); delay(500);          // look left
    long left = distance();
    eye.write(90);  delay(300);

    if (right > left) {
      drive(SPEED, -SPEED);              // turn right in place
    } else {
      drive(-SPEED, SPEED);              // turn left in place
    }
    delay(TURN_MS);
    drive(0, 0);
  }
}
