// ElectroDai - Relay module: switch a 12 V DC lamp
// Relay IN -> D7, button between D2 and GND

const int RELAY_PIN = 7;
const int BUTTON_PIN = 2;

// Most modules are active LOW. If yours works the opposite way, set false.
const bool RELAY_ACTIVE_LOW = true;

bool relayOn = false;
bool lastReading = HIGH;
bool stableButton = HIGH;
unsigned long lastChange = 0;

void setRelay(bool on) {
  relayOn = on;
  bool level = RELAY_ACTIVE_LOW ? !on : on;
  digitalWrite(RELAY_PIN, level ? HIGH : LOW);
  Serial.println(on ? "Relay ON" : "Relay OFF");
}

void setup() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // button pressed = LOW
  Serial.begin(9600);
  setRelay(false);                     // start with the load off
}

void loop() {
  bool button = digitalRead(BUTTON_PIN);

  unsigned long now = millis();
  if (button != lastReading) {
    lastReading = button;
    lastChange = now;
  }
  if (button != stableButton && now - lastChange >= 50) {
    stableButton = button;
    if (stableButton == LOW) setRelay(!relayOn);
  }
}
