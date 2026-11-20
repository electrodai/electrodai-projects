// ESP32 deep sleep: timer + button wake, RTC counter - compiles, not yet tested on hardware
#include <Arduino.h>

const int  SENSOR_PIN = 34;                    // ADC1
const gpio_num_t BUTTON_PIN = GPIO_NUM_33;     // RTC GPIO, external pull-up
const uint64_t SLEEP_SECONDS = 60;

RTC_DATA_ATTR int bootCount = 0;               // survives deep sleep
RTC_DATA_ATTR int lastReading = -1;

void printWakeReason() {
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  switch (cause) {
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Woke by timer");  break;
    case ESP_SLEEP_WAKEUP_EXT0:  Serial.println("Woke by button"); break;
    default:                     Serial.println("Power-on or reset"); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);                    // give Serial time to start

  pinMode(BUTTON_PIN, INPUT);   // external 10 kΩ pull-up
  bootCount++;
  Serial.printf("Boot number: %d\n", bootCount);
  printWakeReason();

  int reading = analogRead(SENSOR_PIN);
  Serial.printf("Sensor: %d (previous: %d)\n", reading, lastReading);
  lastReading = reading;

  // Here you would connect Wi-Fi, send the reading, then disconnect.

  esp_sleep_enable_timer_wakeup(SLEEP_SECONDS * 1000000ULL);
  esp_sleep_enable_ext0_wakeup(BUTTON_PIN, 0);   // wake when GPIO 33 goes LOW

  while (digitalRead(BUTTON_PIN) == LOW) delay(20); // wait for release
  Serial.println("Going to sleep");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
  // Never reached: the ESP32 restarts from setup() after each wake
}
