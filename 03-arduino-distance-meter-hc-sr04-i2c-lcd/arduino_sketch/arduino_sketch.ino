/*
  ElectroDai Project 01 - Ultrasonic Distance Meter
  Arduino Uno + HC-SR04 + 16x2 I2C LCD
  Library: "LiquidCrystal I2C" by Frank de Brabander (Library Manager)

  Wiring:
    HC-SR04: VCC->5V, GND->GND, Trig->D9, Echo->D10
    I2C LCD: VCC->5V, GND->GND, SDA->A4, SCL->A5
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ----- Pins -----
const uint8_t TRIG_PIN = 9;    // HC-SR04 Trig
const uint8_t ECHO_PIN = 10;   // HC-SR04 Echo

// ----- LCD -----
// Most backpacks (PCF8574T chip) use address 0x27.
// Backpacks with the PCF8574AT chip use 0x3F.
// If the backlight is on but no text appears, change 0x27 to 0x3F.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----- Calibration -----
const float AIR_TEMP_C    = 25.0;  // room temperature in degrees C (sound is faster in warm air)
const float CAL_OFFSET_CM = 0.0;   // fine correction after checking with a ruler

// ----- Limits -----
const unsigned long ECHO_TIMEOUT_US = 30000UL; // stop waiting after 30 ms (about 5 m)
const float MIN_CM = 2.0;                      // HC-SR04 useful range
const float MAX_CM = 400.0;

float soundSpeedCmPerUs;  // calculated in setup()

// Send one ping and return distance in cm, or -1 if no echo came back
float readOnceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);      // 10 us pulse starts a measurement
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long echoUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (echoUs == 0) {
    return -1.0;                     // timeout: nothing in range
  }
  // divide by 2 because the sound goes to the object and back
  return (echoUs * soundSpeedCmPerUs) / 2.0 + CAL_OFFSET_CM;
}

// Take several readings and average only the valid ones
float readAverageCm(uint8_t samples) {
  float total = 0.0;
  uint8_t good = 0;
  for (uint8_t i = 0; i < samples; i++) {
    float d = readOnceCm();
    if (d >= MIN_CM && d <= MAX_CM) {
      total += d;
      good++;
    }
    delay(60);                       // HC-SR04 needs about 60 ms between pings
  }
  if (good == 0) {
    return -1.0;
  }
  return total / good;
}

// Print text on LCD line 2, padded with spaces so old characters are erased
void printLine2(const char *text) {
  char buf[17];                      // 16 characters + end-of-string
  snprintf(buf, sizeof(buf), "%-16s", text);
  lcd.setCursor(0, 1);
  lcd.print(buf);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(9600);

  // Speed of sound: 331.3 + 0.606 x temperature (m/s), converted to cm per microsecond
  soundSpeedCmPerUs = (331.3 + 0.606 * AIR_TEMP_C) / 10000.0;

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ElectroDai");
  lcd.setCursor(0, 1);
  lcd.print("Distance Meter");
  delay(1500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
}

void loop() {
  float distance = readAverageCm(3);

  if (distance < 0) {
    printLine2("Out of range");
    Serial.println("Out of range");
  } else {
    char number[8];
    char line[17];
    dtostrf(distance, 5, 1, number);           // e.g. " 25.4" (1 decimal place)
    snprintf(line, sizeof(line), "%s cm", number);
    printLine2(line);

    Serial.print(distance, 1);
    Serial.println(" cm");
  }

  delay(100);
}
