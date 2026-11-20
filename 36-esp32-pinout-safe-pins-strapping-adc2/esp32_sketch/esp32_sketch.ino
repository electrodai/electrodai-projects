// ADC1 read with Wi-Fi connected - compiles, not yet tested on hardware
#include <WiFi.h>

const char* WIFI_SSID = "YourWiFi";
const char* WIFI_PASS = "YourPassword";
const int POT_PIN = 34;   // ADC1 pin: works with Wi-Fi on

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int raw = analogRead(POT_PIN);            // 0 to 4095
  int mv  = analogReadMilliVolts(POT_PIN);  // calibrated millivolts
  Serial.printf("raw=%d  mV=%d\n", raw, mv);
  delay(500);
}
