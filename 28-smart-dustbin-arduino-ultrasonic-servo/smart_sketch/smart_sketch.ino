// Smart dustbin - compiles, not yet tested on hardware
#include <Servo.h>

const int HAND_TRIG = 9, HAND_ECHO = 10;
const int FULL_TRIG = 7, FULL_ECHO = 8;
const int SERVO_PIN = 6, LED_READY = 4, LED_FULL = 5;

const int OPEN_CM   = 20;     // open when a hand is closer than this
const int CLOSE_CM  = 30;     // keep open until farther than this
const bool USE_FULL_SENSOR = true; // set false if the second sensor is omitted
const int FULL_CM   = 10;     // bin counted full if waste is closer than this
const unsigned long HOLD_MS = 3000;

const int ANGLE_CLOSED = 0;
const int ANGLE_OPEN   = 95;  // tune for your lid

Servo lid;
bool isOpen = false;
unsigned long lastSeen = 0;

long readCm(int trig, int echo) {
  digitalWrite(trig, LOW);  delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  unsigned long us = pulseIn(echo, HIGH, 25000UL);
  delay(60);                     // prevent old echoes reaching the next ping
  if (us == 0 || us > 23200UL) return 401; // no usable echo
  return us / 58;
}

// median of 3
long median3(int trig, int echo) {
  long a = readCm(trig, echo), b = readCm(trig, echo), c = readCm(trig, echo);
  if ((a >= b && a <= c) || (a <= b && a >= c)) return a;
  if ((b >= a && b <= c) || (b <= a && b >= c)) return b;
  return c;
}

void setup() {
  pinMode(HAND_TRIG, OUTPUT); pinMode(HAND_ECHO, INPUT);
  pinMode(FULL_TRIG, OUTPUT); pinMode(FULL_ECHO, INPUT);
  pinMode(LED_READY, OUTPUT); pinMode(LED_FULL, OUTPUT);
  lid.attach(SERVO_PIN);
  lid.write(ANGLE_CLOSED);
  digitalWrite(LED_READY, HIGH);
}

void loop() {
  long hand = median3(HAND_TRIG, HAND_ECHO);

  if (!isOpen && hand < OPEN_CM) {
    lid.write(ANGLE_OPEN);
    isOpen = true;
    lastSeen = millis();
  }
  if (isOpen) {
    if (hand < CLOSE_CM) lastSeen = millis();       // someone still near
    if (millis() - lastSeen > HOLD_MS) {
      lid.write(ANGLE_CLOSED);
      isOpen = false;
      delay(600);                                    // let the lid settle
    }
  }

  // Fullness check only when the lid is closed
  if (USE_FULL_SENSOR && !isOpen) {
    long waste = median3(FULL_TRIG, FULL_ECHO);
    digitalWrite(LED_FULL, waste < FULL_CM ? HIGH : LOW);
  }
  delay(100);
}
