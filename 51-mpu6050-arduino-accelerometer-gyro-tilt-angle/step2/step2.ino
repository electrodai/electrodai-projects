// MPU6050 tilt angle with complementary filter - not compiled yet
#include <Wire.h>

const int MPU = 0x68;
float gyroOffsetX = 0, gyroOffsetY = 0;
float roll = 0, pitch = 0;
unsigned long lastMicros;

void readRaw(int16_t &ax, int16_t &ay, int16_t &az, int16_t &gx, int16_t &gy, int16_t &gz) {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);                    // ACCEL_XOUT_H, start of the data block
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);     // accel (6), temperature (2), gyro (6)
  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();
  Wire.read(); Wire.read();            // skip temperature
  gx = Wire.read() << 8 | Wire.read();
  gy = Wire.read() << 8 | Wire.read();
  gz = Wire.read() << 8 | Wire.read();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);                    // PWR_MGMT_1
  Wire.write(0);                       // wake up (it starts in sleep mode)
  Wire.endTransmission(true);

  // gyro offset: keep the sensor still during this
  Serial.println("Calibrating, keep still...");
  int16_t ax, ay, az, gx, gy, gz;
  for (int i = 0; i < 500; i++) {
    readRaw(ax, ay, az, gx, gy, gz);
    gyroOffsetX += gx;
    gyroOffsetY += gy;
    delay(3);
  }
  gyroOffsetX /= 500.0;
  gyroOffsetY /= 500.0;

  // start the filter at the accelerometer angle
  readRaw(ax, ay, az, gx, gy, gz);
  roll  = atan2((float)ay, (float)az) * 180.0 / PI;
  pitch = atan2(-(float)ax, sqrt((float)ay * ay + (float)az * az)) * 180.0 / PI;
  lastMicros = micros();
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  readRaw(ax, ay, az, gx, gy, gz);

  unsigned long now = micros();
  float dt = (now - lastMicros) / 1000000.0;
  lastMicros = now;

  float accRoll  = atan2((float)ay, (float)az) * 180.0 / PI;
  float accPitch = atan2(-(float)ax, sqrt((float)ay * ay + (float)az * az)) * 180.0 / PI;

  float rateX = (gx - gyroOffsetX) / 131.0;   // deg/s around X
  float rateY = (gy - gyroOffsetY) / 131.0;   // deg/s around Y

  roll  = 0.98 * (roll  + rateX * dt) + 0.02 * accRoll;
  pitch = 0.98 * (pitch + rateY * dt) + 0.02 * accPitch;

  Serial.print("roll:");   Serial.print(roll, 1);
  Serial.print(" pitch:"); Serial.println(pitch, 1);
  delay(10);
}
