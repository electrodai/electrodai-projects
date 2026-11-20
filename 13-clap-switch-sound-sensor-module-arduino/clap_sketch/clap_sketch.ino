// ElectroDai - Double-clap switch with a sound sensor module
// Sound module DO -> D2, LED (with 220 ohm resistor) -> D8

const int SOUND_PIN = 2;   // digital output of the sound module
const int LED_PIN = 8;     // LED

// Level on DO when a clap is heard. Many modules give HIGH on sound.
// If your LED toggles at random or never, try changing HIGH to LOW.
const int ACTIVE_LEVEL = HIGH;

const unsigned long LOCKOUT_MS = 150;   // ignore echoes after a clap
const unsigned long MAX_GAP_MS = 800;   // second clap must come within this time

bool wasActive = false;
bool ledOn = false;                     // current LED state
bool waitingSecondClap = false;         // true after the first clap
unsigned long firstClapTime = 0;        // when the first clap happened
unsigned long lastClapTime = 0;         // when the last clap was accepted

void setup() {
  pinMode(SOUND_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  wasActive = (digitalRead(SOUND_PIN) == ACTIVE_LEVEL);
  Serial.begin(9600);                   // for debugging in Serial Monitor
}

void loop() {
  unsigned long now = millis();

  if (waitingSecondClap && now - firstClapTime > MAX_GAP_MS) {
    waitingSecondClap = false;
    Serial.println("Timeout");
  }

  bool active = (digitalRead(SOUND_PIN) == ACTIVE_LEVEL);
  bool newClap = active && !wasActive;
  wasActive = active;
  if (newClap && now - lastClapTime >= LOCKOUT_MS) {
    lastClapTime = now;

    if (!waitingSecondClap) {
      // First clap: start the timer
      waitingSecondClap = true;
      firstClapTime = now;
      Serial.println("Clap 1");
    } else {
      // Second clap inside the window: toggle the LED
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      waitingSecondClap = false;
      Serial.println(ledOn ? "Clap 2 -> LED ON" : "Clap 2 -> LED OFF");
    }
  }

}
