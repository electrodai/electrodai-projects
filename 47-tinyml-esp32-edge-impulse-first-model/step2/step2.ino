// Run the gesture model on ESP32 - not compiled yet
#include <gestures_inferencing.h>   // change to your project's header
#include <Wire.h>

const int MPU = 0x68;
const float SCALE = 9.81 / 8192.0;
float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  Wire.beginTransmission(MPU); Wire.write(0x6B); Wire.write(0); Wire.endTransmission();
  Wire.beginTransmission(MPU); Wire.write(0x1C); Wire.write(0x08); Wire.endTransmission();
  Serial.println("Ready. Perform a gesture.");
}

void readAccel(float *out) {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  for (int i = 0; i < 3; i++) {
    int16_t v = Wire.read() << 8 | Wire.read();
    out[i] = v * SCALE;
  }
}

void loop() {
  // fill one window of samples at the model's sampling interval
  for (size_t i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i += 3) {
    unsigned long next = millis() + EI_CLASSIFIER_INTERVAL_MS;
    readAccel(&buffer[i]);
    while (millis() < next) { }
  }

  signal_t signal;
  numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
  ei_impulse_result_t result;
  if (run_classifier(&signal, &result, false) != EI_IMPULSE_OK) return;

  const char *best = "";
  float bestScore = 0;
  for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
    if (result.classification[i].value > bestScore) {
      bestScore = result.classification[i].value;
      best = result.classification[i].label;
    }
  }
  if (bestScore > 0.8) {             // only act when the model is confident
    Serial.print(best); Serial.print("  "); Serial.println(bestScore, 2);
  }
}
