// ElectroDai - ESP32 + DHT11 + Blynk IoT
// Replace the placeholders with YOUR values from Blynk. Keep them private.

#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

#define DHTPIN 4
#define DHTTYPE DHT11
const int LED_PIN = 23;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Runs every few seconds: read the sensor and send the values
void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT read failed");
    return;                          // skip this round
  }
  if (!Blynk.connected()) return;
  Blynk.virtualWrite(V0, t);         // Temperature datastream
  Blynk.virtualWrite(V1, h);         // Humidity datastream
  Serial.printf("Temp %.1f C, Humidity %.0f %%\n", t, h);
}

// Runs when the switch on V2 changes in the app
BLYNK_WRITE(V2) {
  int value = param.asInt();         // 1 = ON, 0 = OFF
  digitalWrite(LED_PIN, value ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);   // connect to Wi-Fi and Blynk
  timer.setInterval(5000L, sendSensorData);    // every 5 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
