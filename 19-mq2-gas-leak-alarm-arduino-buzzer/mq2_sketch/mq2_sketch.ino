// ElectroDai - MQ-2 gas alarm (LEARNING PROJECT, not a certified safety device)
// MQ-2 AO -> A0, buzzer -> D8, LED (via 220 ohm) -> D7

const int GAS_PIN = A0;
const int BUZZER_PIN = 8;
const int LED_PIN = 7;

const unsigned long WARMUP_MS = 180000;  // 3 minutes of heater warm-up
const int MARGIN = 120;                  // alarm when reading > baseline + MARGIN

int baseline = 0;
int threshold = 0;

int readAverage() {
  long sum = 0;
  for (int i = 0; i < 20; i++) {         // average 20 samples to smooth noise
    sum += analogRead(GAS_PIN);
    delay(25);
  }
  return sum / 20;
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Warming up MQ-2. Keep in clean air...");

  unsigned long start = millis();
  while (millis() - start < WARMUP_MS) {
    digitalWrite(LED_PIN, (millis() / 500) % 2);  // blink while warming
    delay(20);
  }
  digitalWrite(LED_PIN, LOW);

  baseline = readAverage();
  threshold = baseline + MARGIN;
  if (threshold >= 1023) {
    Serial.println("Baseline too high. Check sensor and restart in clean air.");
    while (true) {
      digitalWrite(LED_PIN, (millis() / 250) % 2);
      delay(20);
    }
  }
  Serial.print("Baseline: ");
  Serial.print(baseline);
  Serial.print("  Alarm threshold: ");
  Serial.println(threshold);
}

void loop() {
  int value = readAverage();
  Serial.print("Gas reading: ");
  Serial.println(value);

  if (value > threshold) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }
}
