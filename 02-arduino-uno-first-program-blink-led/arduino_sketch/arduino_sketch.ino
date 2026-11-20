/*
  ElectroDai - Arduino Uno First Blink
  Blinks the built-in LED (pin 13) and an external LED on pin 8 together.
  Wiring: D8 -> 220 ohm resistor -> LED long leg; LED short leg -> GND
*/

const int EXTERNAL_LED_PIN = 8;          // external LED through 220 ohm resistor
const unsigned long ON_TIME_MS  = 500;   // how long the LED stays ON (milliseconds)
const unsigned long OFF_TIME_MS = 500;   // how long the LED stays OFF

void setup() {
  // setup() runs once when the board powers up or resets
  pinMode(LED_BUILTIN, OUTPUT);          // LED_BUILTIN is pin 13 on the Uno
  pinMode(EXTERNAL_LED_PIN, OUTPUT);

  Serial.begin(9600);                    // open the Serial Monitor at 9600 baud to see messages
  Serial.println("ElectroDai blink started");
}

void loop() {
  // loop() runs again and again forever
  digitalWrite(LED_BUILTIN, HIGH);       // HIGH = 5 V = LED on
  digitalWrite(EXTERNAL_LED_PIN, HIGH);
  Serial.println("LED ON");
  delay(ON_TIME_MS);                     // wait

  digitalWrite(LED_BUILTIN, LOW);        // LOW = 0 V = LED off
  digitalWrite(EXTERNAL_LED_PIN, LOW);
  Serial.println("LED OFF");
  delay(OFF_TIME_MS);
}
