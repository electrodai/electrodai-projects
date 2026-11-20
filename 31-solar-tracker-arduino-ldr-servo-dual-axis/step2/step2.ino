// Dual axis solar tracker - compiles, not yet tested on hardware
#include <Servo.h>

const bool DUAL_AXIS = true;
Servo pan, tilt;
const int PAN_PIN = 9, TILT_PIN = 10;

int panAngle  = 90;
int tiltAngle = 90;

const int PAN_MIN = 20,  PAN_MAX = 160;
const int TILT_MIN = 40, TILT_MAX = 140;
const int TOLERANCE = 30;      // ignore small differences (difference between two sums)
const int STEP = 1;            // degrees per move
const int LOOP_DELAY = 80;     // ms; slower is gentler on the servos

int readAvg(int pin) {
  long sum = 0;
  for (int i = 0; i < 8; i++) sum += analogRead(pin);
  return sum / 8;
}

void setup() {
  pan.attach(PAN_PIN);
  pan.write(panAngle);
  if (DUAL_AXIS) {
    tilt.attach(TILT_PIN);
    tilt.write(tiltAngle);
  }
  Serial.begin(9600);
  delay(500);
}

void loop() {
  int tl = readAvg(A0);
  int tr = readAvg(A1);
  int bl = DUAL_AXIS ? readAvg(A2) : 0;
  int br = DUAL_AXIS ? readAvg(A3) : 0;

  int left   = tl + bl;
  int right  = tr + br;
  int top    = tl + tr;
  int bottom = bl + br;

  int panError  = left - right;      // positive: left side brighter
  int tiltError = top  - bottom;     // positive: top brighter

  // Directions below depend on how the servos are mounted.
  // If the panel turns away from the light, flip the + and - signs.
  if (abs(panError) > TOLERANCE) {
    panAngle += (panError > 0) ? STEP : -STEP;
    panAngle = constrain(panAngle, PAN_MIN, PAN_MAX);
    pan.write(panAngle);
  }
  if (DUAL_AXIS && abs(tiltError) > TOLERANCE) {
    tiltAngle += (tiltError > 0) ? STEP : -STEP;
    tiltAngle = constrain(tiltAngle, TILT_MIN, TILT_MAX);
    tilt.write(tiltAngle);
  }

  Serial.print(panAngle); Serial.print(" ");
  Serial.println(tiltAngle);
  delay(LOOP_DELAY);
}
