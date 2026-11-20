// Data forwarder sketch: MPU6050 accel at 50 Hz - not compiled yet
#include <Wire.h>

const int MPU = 0x68;
const float SCALE = 9.81 / 8192.0;     // +/-4 g range: 8192 LSB per g
const unsigned long INTERVAL_MS = 20;  // 50 Hz

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); Wire.write(0);     // wake up
  Wire.endTransmission();
  Wire.beginTransmission(MPU);
  Wire.write(0x1C); Wire.write(0x08);  // ACCEL_CONFIG: +/-4 g
  Wire.endTransmission();
}

void readAccel(float &x, float &y, float &z) {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  int16_t ax = Wire.read() << 8 | Wire.read();
  int16_t ay = Wire.read() << 8 | Wire.read();
  int16_t az = Wire.read() << 8 | Wire.read();
  x = ax * SCALE; y = ay * SCALE; z = az * SCALE;
}

void loop() {
  static unsigned long last = 0;
  if (millis() - last >= INTERVAL_MS) {
    last = millis();
    float x, y, z;
    readAccel(x, y, z);
    Serial.print(x); Serial.print(',');
    Serial.print(y); Serial.print(',');
    Serial.println(z);
  }
}
