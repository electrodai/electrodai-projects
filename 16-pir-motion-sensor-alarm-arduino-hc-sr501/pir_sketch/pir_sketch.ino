// ElectroDai - PIR motion alarm (HC-SR501)
// PIR OUT -> D2, LED (via 220 ohm) -> D7, active buzzer -> D8

const int PIR_PIN = 2;
const int LED_PIN = 7;
const int BUZZER_PIN = 8;

const unsigned long WARMUP_MS = 60000;   // PIR settling time at power-up
const unsigned long ALARM_MS = 5000;     // how long the alarm stays on after PIR OUT goes LOW

unsigned long alarmStart = 0;
bool alarmOn = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);

  Serial.println("Warming up the PIR sensor (60 s). Keep still...");
  // Blink the LED during warm-up so you know the alarm is not ready
  unsigned long start = millis();
  while (millis() - start < WARMUP_MS) {
    digitalWrite(LED_PIN, (millis() / 500) % 2);
    delay(10);
  }
  digitalWrite(LED_PIN, LOW);
  Serial.println("Alarm armed.");
}

void loop() {
  unsigned long now = millis();

  // Motion detected: start (or restart) the alarm
  if (digitalRead(PIR_PIN) == HIGH) {
    if (!alarmOn) Serial.println("Motion detected!");
    alarmOn = true;
    alarmStart = now;
  }

  if (alarmOn) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    // Stop after PIR OUT has been LOW for ALARM_MS
    if (now - alarmStart > ALARM_MS) {
      alarmOn = false;
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("Alarm stopped.");
    }
  }
}
