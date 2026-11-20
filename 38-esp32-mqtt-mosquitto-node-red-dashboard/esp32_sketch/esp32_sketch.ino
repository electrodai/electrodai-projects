// ESP32 DHT11 -> MQTT publisher - not compiled yet
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* WIFI_SSID   = "YourWiFi";
const char* WIFI_PASS   = "YourPassword";
const char* MQTT_HOST   = "192.168.1.50";   // your broker's IP
const int   MQTT_PORT   = 1883;
const char* MQTT_USER   = "esp32user";
const char* MQTT_PASS   = "yourpassword";
const char* TOPIC       = "home/room1/dht";

const int DHT_PIN = 4;
DHT dht(DHT_PIN, DHT11);

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
unsigned long lastSend = 0;

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWi-Fi OK, IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting to MQTT... ");
    String clientId = "esp32-room1-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (mqtt.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, state=");
      Serial.println(mqtt.state());
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectWiFi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();                                  // keep the connection alive

  if (millis() - lastSend > 10000) {            // every 10 seconds
    lastSend = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (isnan(t) || isnan(h)) {
      Serial.println("DHT read failed");
      return;
    }
    char payload[64];
    snprintf(payload, sizeof(payload), "{\"t\":%.1f,\"h\":%.0f}", t, h);
    mqtt.publish(TOPIC, payload);
    Serial.print("Sent: ");
    Serial.println(payload);
  }
}
