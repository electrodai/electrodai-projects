// ESP32 tank level monitor with night leak check - not compiled yet
#include <WiFi.h>
#include <WebServer.h>
#include <time.h>

const char* WIFI_SSID = "YourWiFi";
const char* WIFI_PASS = "YourPassword";

const int TRIG_PIN = 26;
const int ECHO_PIN = 27;       // through 1k/2k divider
const int BUZZER_PIN = 25;     // via NPN transistor

const float EMPTY_CM = 120.0;  // measure on YOUR tank
const float FULL_CM  = 30.0;   // must be beyond the blind zone
const float LEAK_DROP_PERCENT = 5.0;
const int   QUIET_START_HOUR = 1;   // 1 AM
const int   QUIET_END_HOUR   = 4;   // 4 AM

WebServer server(80);
float levelPercent = -1;
float quietStartLevel = -1;
bool  leakSuspected = false;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long us = pulseIn(ECHO_PIN, HIGH, 30000);  // timeout about 5 m
  if (us == 0) return -1;                             // no echo
  return us * 0.0343 / 2.0;
}

float medianDistance() {
  float v[5];
  int count = 0;
  for (int i = 0; i < 5; i++) {
    float distance = readDistanceCm();
    if (distance >= 0) v[count++] = distance;
    delay(60);
  }
  if (count < 3) return -1;                 // too few valid echoes
  for (int i = 0; i < count - 1; i++)
    for (int j = i + 1; j < count; j++)
      if (v[j] < v[i]) { float t = v[i]; v[i] = v[j]; v[j] = t; }
  return (v[(count - 1) / 2] + v[count / 2]) / 2.0;
}

float toPercent(float d) {
  float p = (EMPTY_CM - d) / (EMPTY_CM - FULL_CM) * 100.0;
  return constrain(p, 0, 100);
}

void handleRoot() {
  String html = "<html><head><meta name='viewport' content='width=device-width'>"
                "<meta http-equiv='refresh' content='10'></head><body style='font-family:sans-serif'>"
                "<h2>Water tank</h2>";
  if (levelPercent < 0) html += "<p>Sensor error</p>";
  else html += "<p style='font-size:48px'>" + String(levelPercent, 0) + "%</p>";
  if (leakSuspected) html += "<p style='color:red'>Possible leak: level dropped overnight</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.print("\nOpen http://"); Serial.println(WiFi.localIP());

  configTime(5 * 3600 + 45 * 60, 0, "pool.ntp.org");   // Nepal time, UTC+5:45
  server.on("/", handleRoot);
  server.begin();
}

unsigned long lastRead = 0;

void loop() {
  server.handleClient();
  if (millis() - lastRead < 5000) return;
  lastRead = millis();

  float d = medianDistance();
  levelPercent = (d < 0) ? -1 : toPercent(d);
  Serial.printf("distance=%.1f cm level=%.0f%%\n", d, levelPercent);

  struct tm now;
  if (levelPercent >= 0 && getLocalTime(&now)) {
    if (now.tm_hour == QUIET_START_HOUR && quietStartLevel < 0) {
      quietStartLevel = levelPercent;            // remember level at 1 AM
    }
    if (now.tm_hour == QUIET_END_HOUR && quietStartLevel >= 0) {
      leakSuspected = (quietStartLevel - levelPercent) > LEAK_DROP_PERCENT;
      quietStartLevel = -1;                      // reset for next night
    }
  }
  digitalWrite(BUZZER_PIN, leakSuspected ? HIGH : LOW);
}
