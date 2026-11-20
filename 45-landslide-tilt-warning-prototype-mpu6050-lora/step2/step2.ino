// LoRa gateway with buzzer - not compiled yet
#include <SPI.h>
#include <LoRa.h>

const int BUZZER = 25;
const int LED = 27;
const unsigned long SILENT_MS = 180000;   // 3 missed heartbeats
unsigned long lastHeard = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  LoRa.setPins(4, 14, 26);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed");
    while (true) delay(1000);
  }
  lastHeard = millis();
}

void loop() {
  int size = LoRa.parsePacket();
  if (size) {
    String msg = LoRa.readString();
    lastHeard = millis();
    Serial.print(msg);
    Serial.print("  RSSI ");
    Serial.println(LoRa.packetRssi());

    if (msg.indexOf("ALERT") > 0) {
      digitalWrite(LED, HIGH);
      for (int i = 0; i < 10; i++) {
        digitalWrite(BUZZER, HIGH); delay(200);
        digitalWrite(BUZZER, LOW);  delay(200);
      }
    }
  }
  if (millis() - lastHeard > SILENT_MS) {
    Serial.println("WARNING: node silent - check battery or link");
    lastHeard = millis();
  }
}
