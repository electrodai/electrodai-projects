// ElectroDai - Automatic plant watering
// Soil sensor AO -> A0, relay IN -> D7

const int SENSOR_PIN = A0;
const int RELAY_PIN = 7;

// Calibration: put YOUR readings here (see Serial Monitor)
const int DRY_VALUE = 800;   // reading in air or very dry soil
const int WET_VALUE = 350;   // reading in a glass of water

const int START_BELOW_PERCENT = 35;       // water when moisture is below 35 %
const unsigned long PUMP_MAX_MS = 5000;   // pump runs 5 s at most
const unsigned long REST_MS = 60000;      // wait 60 s before checking again

const bool RELAY_ACTIVE_LOW = true;       // most relay modules are active LOW

void pump(bool on) {
  bool level = RELAY_ACTIVE_LOW ? !on : on;
  digitalWrite(RELAY_PIN, level ? HIGH : LOW);
}

int readMoisturePercent(int raw) {
  // Convert to 0-100 %. map() works in either direction.
  int percent = map(raw, DRY_VALUE, WET_VALUE, 0, 100);
  return constrain(percent, 0, 100);
}

void setup() {
  pump(false);                  // preset the inactive output level on the Uno
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int raw = analogRead(SENSOR_PIN);
  if (DRY_VALUE == WET_VALUE) {
    pump(false);
    Serial.println("Calibration error: dry and wet values must differ");
    delay(2000);
    return;
  }
  int percent = readMoisturePercent(raw);
  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("  Moisture: ");
  Serial.print(percent);
  Serial.println(" %");

  if (percent < START_BELOW_PERCENT) {
    Serial.println("Soil dry - pump ON");
    pump(true);
    delay(PUMP_MAX_MS);         // short, fixed watering time
    pump(false);
    Serial.println("Pump OFF - resting");
    delay(REST_MS);             // let water soak in
  } else {
    delay(2000);
  }
}
