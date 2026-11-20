// Two-sensor line follower - compiles, not yet tested on hardware
const int LEFT_IR  = 2;
const int RIGHT_IR = 3;

const int ENA = 5;   // left motor speed (PWM)
const int IN1 = 7;
const int IN2 = 8;
const int IN3 = 9;
const int IN4 = 10;
const int ENB = 6;   // right motor speed (PWM)

// Change to LOW if your module outputs LOW over black
const int ON_LINE = HIGH;

const int BASE_SPEED = 130;  // 0-255, start low
const int TURN_SPEED = 160;  // speed of the outer wheel when turning

void setup() {
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
}

// speeds: positive = forward, negative = backward
void drive(int left, int right) {
  digitalWrite(IN1, left  > 0);  digitalWrite(IN2, left  < 0);
  digitalWrite(IN3, right > 0);  digitalWrite(IN4, right < 0);
  analogWrite(ENA, abs(left));
  analogWrite(ENB, abs(right));
}

void loop() {
  bool leftOnLine  = (digitalRead(LEFT_IR)  == ON_LINE);
  bool rightOnLine = (digitalRead(RIGHT_IR) == ON_LINE);

  if (!leftOnLine && !rightOnLine) {
    drive(BASE_SPEED, BASE_SPEED);        // assume the line is between the sensors
  } else if (leftOnLine && !rightOnLine) {
    drive(0, TURN_SPEED);                 // line on left: turn left
  } else if (!leftOnLine && rightOnLine) {
    drive(TURN_SPEED, 0);                 // line on right: turn right
  } else {
    drive(0, 0);                          // both on black: stop
  }
}
