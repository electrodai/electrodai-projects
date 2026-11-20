// I2C scanner - not compiled yet
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("Scan done");
}

void loop() {}
