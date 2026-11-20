// 4-servo robotic arm with potentiometers - not compiled yet
#include <Servo.h>

const int NUM = 4;
const int POT_PIN[NUM]   = {A0, A1, A2, A3};
const int SERVO_PIN[NUM] = {3, 5, 6, 9};

// Safe angle range for each joint: base, shoulder, elbow, gripper
const int MIN_ANGLE[NUM] = {10, 30, 20, 40};
const int MAX_ANGLE[NUM] = {170, 150, 160, 110};

const int DEADBAND = 2;      // ignore changes smaller than this (degrees)
const int STEP = 2;          // max degrees moved per loop, for smooth motion

Servo joint[NUM];
int current[NUM];

int readAngle(int i) {
  long total = 0;
  for (int k = 0; k < 8; k++) total += analogRead(POT_PIN[i]);  // average 8 reads
  int raw = total / 8;
  return map(raw, 0, 1023, MIN_ANGLE[i], MAX_ANGLE[i]);
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM; i++) {
    current[i] = readAngle(i);       // start where the knobs are
    joint[i].attach(SERVO_PIN[i]);
    joint[i].write(current[i]);
    delay(300);                      // start servos one at a time to limit the current surge
  }
}

void loop() {
  for (int i = 0; i < NUM; i++) {
    int target = readAngle(i);
    int diff = target - current[i];

    if (abs(diff) > DEADBAND) {
      current[i] += constrain(diff, -STEP, STEP);  // move a little toward the target
      joint[i].write(current[i]);
    }
  }
  delay(15);
}
