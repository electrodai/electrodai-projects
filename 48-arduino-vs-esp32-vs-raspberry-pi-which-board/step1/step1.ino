// Arduino: receive one-letter commands from the Pi - compiles, not yet tested on hardware
const int LED = 13;

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'F') { digitalWrite(LED, HIGH); Serial.println("forward"); }
    if (c == 'S') { digitalWrite(LED, LOW);  Serial.println("stop"); }
    // later: replace the LED with motor driver commands
  }
}
