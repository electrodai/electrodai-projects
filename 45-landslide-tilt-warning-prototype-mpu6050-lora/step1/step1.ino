// Tilt sensor node (learning prototype) - not compiled yet
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>

const int MPU = 0x68;
const long LORA_FREQ = 433E6;          // must match your module and local rules

const float THRESHOLD_DEG = 5.0;       // change from baseline that counts as tilt
const unsigned long HOLD_MS = 10000;   // tilt must last this long
const unsigned long HEARTBEAT_MS = 60000;

float baseline = 0;
unsigned long tiltStart = 0;
unsigned long lastBeat = 0;
bool alerted = false;

float readTiltDeg() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);                    // ACCEL_XOUT_H
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  int16_t ax = Wire.read() << 8 | Wire.read();
  int16_t ay = Wire.read() << 8 | Wire.read();
  int16_t az = Wire.read() << 8 | Wire.read();
  // tilt of the stick from vertical, using all three axes
  float horiz = sqrt((float)ax * ax + (float)ay * ay);
  return atan2(horiz, (float)az) * 180.0 / PI;
}

float averageTilt(int n) {
  float sum = 0;
  for (int i = 0; i < n; i++) { sum += readTiltDeg(); delay(10); }
  return sum / n;
}

void sendMsg(String msg) {
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();
  Serial.println("Sent: " + msg);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);                    // PWR_MGMT_1
  Wire.write(0);                       // wake the sensor
  Wire.endTransmission(true);

  LoRa.setPins(4, 14, 26);             // NSS, RESET, DIO0
  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init failed");
    while (true) delay(1000);
  }
  delay(2000);                         // let the stick settle
  baseline = averageTilt(100);
  sendMsg("N1,START," + String(baseline, 1));
}

void loop() {
  float change = fabs(averageTilt(20) - baseline);

  if (change > THRESHOLD_DEG) {
    if (tiltStart == 0) tiltStart = millis();
    if (!alerted && millis() - tiltStart > HOLD_MS) {
      sendMsg("N1,ALERT," + String(change, 1));
      alerted = true;
    }
  } else {
    tiltStart = 0;
    alerted = false;                   // back to normal, allow a new alert
  }

  if (millis() - lastBeat > HEARTBEAT_MS) {
    sendMsg("N1,OK," + String(change, 1));
    lastBeat = millis();
  }
  delay(500);
}
