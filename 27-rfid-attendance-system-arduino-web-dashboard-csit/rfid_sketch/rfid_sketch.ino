// RFID reader: prints card UID over serial - compiles, not yet tested on hardware
#include <SPI.h>
#include <MFRC522.h>

const int SS_PIN = 10, RST_PIN = 9;
const int BUZZER = 8, LED_OK = 6, LED_ERR = 7;

MFRC522 rfid(SS_PIN, RST_PIN);
String lastUid = "";
unsigned long lastScan = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_OK, OUTPUT);
  pinMode(LED_ERR, OUTPUT);
}

void loop() {
  // Read the reply from the PC: "OK" or "ERR"
  if (Serial.available()) {
    String reply = Serial.readStringUntil('\n');
    reply.trim();
    if (reply == "OK") { digitalWrite(LED_OK, HIGH); tone(BUZZER, 2000, 150); delay(300); digitalWrite(LED_OK, LOW); }
    else if (reply == "ERR") { digitalWrite(LED_ERR, HIGH); tone(BUZZER, 400, 400); delay(500); digitalWrite(LED_ERR, LOW); }
  }

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  // Ignore the same card read again within 2 seconds
  if (uid != lastUid || millis() - lastScan > 2000) {
    Serial.println(uid);
    lastUid = uid;
    lastScan = millis();
  }
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
