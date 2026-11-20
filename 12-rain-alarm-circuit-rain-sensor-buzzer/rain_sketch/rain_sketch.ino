// ElectroDai - Rain alarm with Arduino Uno
// Rain module: DO -> D2, AO -> A0. Active buzzer on D8.

const int DO_PIN = 2;       // digital output of the rain module
const int AO_PIN = A0;      // analog output of the rain module
const int BUZZER_PIN = 8;   // active buzzer
const int LED_PIN = 13;     // onboard LED

// Most modules give LOW on DO when wet. Change to HIGH if yours is opposite.
const int WET_LEVEL = LOW;

unsigned long lastPrint = 0; // time of the last serial print

void setup() {
  pinMode(DO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  bool raining = (digitalRead(DO_PIN) == WET_LEVEL);

  digitalWrite(BUZZER_PIN, raining ? HIGH : LOW);
  digitalWrite(LED_PIN, raining ? HIGH : LOW);

  // Print the analog value twice per second (typically lower when wet)
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    Serial.print("AO = ");
    Serial.print(analogRead(AO_PIN));
    Serial.print("  Rain: ");
    Serial.println(raining ? "YES" : "no");
  }
}
