// Battery voltage monitor with low-battery alarm - compiles, not yet tested on hardware
const int BAT_PIN    = A0;
const int BUZZER_PIN = 8;
const int LED_PIN    = 7;

const float R1   = 39000.0;  // measured value of top resistor (ohms)
const float R2   = 10000.0;  // measured value of bottom resistor (ohms)
const float VREF = 5.00;     // measured voltage on the 5V pin
const float CAL  = 1.000;    // fine trim after comparing with a multimeter

// Example thresholds for a 12 V lead-acid battery. Change for your battery.
const float LOW_V    = 11.8;  // warning on below this
const float RESET_V  = 12.1;  // warning off above this (hysteresis)

bool lowWarning = false;

float readBattery() {
  long total = 0;
  for (int i = 0; i < 32; i++) {      // average 32 readings to reduce noise
    total += analogRead(BAT_PIN);
    delay(2);
  }
  float raw  = total / 32.0;
  float vPin = raw * VREF / 1023.0;
  return vPin * (R1 + R2) / R2 * CAL;
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  float vBat = readBattery();

  if (!lowWarning && vBat < LOW_V)   lowWarning = true;
  if (lowWarning && vBat > RESET_V)  lowWarning = false;

  digitalWrite(LED_PIN, lowWarning);
  if (lowWarning) {                   // short beep every loop when low
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.print("Battery: ");
  Serial.print(vBat, 2);
  Serial.println(lowWarning ? " V  LOW!" : " V");
  delay(2000);
}
