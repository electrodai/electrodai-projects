// Solar panel voltage logger - not compiled yet
const int PANEL_PIN = A0;
const float R1 = 39000.0;   // top resistor (ohms)
const float R2 = 10000.0;   // bottom resistor (ohms)
const float VREF = 5.0;     // measure your board's 5V pin and put the real value here

void setup() {
  Serial.begin(9600);
  Serial.println("seconds,panel_volts");
}

void loop() {
  long total = 0;
  for (int i = 0; i < 16; i++) {      // average 16 readings
    total += analogRead(PANEL_PIN);
    delay(5);
  }
  float raw = total / 16.0;
  float vPin = raw * VREF / 1023.0;
  float vPanel = vPin * (R1 + R2) / R2;

  Serial.print(millis() / 1000);
  Serial.print(",");
  Serial.println(vPanel, 2);
  delay(10000);                       // one reading every 10 seconds
}
