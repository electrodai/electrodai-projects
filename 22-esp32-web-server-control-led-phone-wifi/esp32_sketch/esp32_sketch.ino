// ElectroDai - ESP32 web server to control an LED
// LED (with 220 ohm resistor) on GPIO 23

#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

const int LED_PIN = 23;
bool ledOn = false;

WebServer server(80);   // HTTP port

// Build the web page. The text changes with the LED state.
String page() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 LED</title></head><body style='font-family:sans-serif;text-align:center'>";
  html += "<h1>ElectroDai ESP32 LED</h1>";
  html += ledOn ? "<p>LED is <b>ON</b></p>" : "<p>LED is <b>OFF</b></p>";
  html += "<p><a href='/on'><button style='font-size:2em;padding:20px'>ON</button></a> ";
  html += "<a href='/off'><button style='font-size:2em;padding:20px'>OFF</button></a></p>";
  html += "</body></html>";
  return html;
}

void handleRoot() { server.send(200, "text/html", page()); }

void handleOn() {
  ledOn = true;
  digitalWrite(LED_PIN, HIGH);
  server.sendHeader("Location", "/");   // go back to the main page
  server.send(303, "text/plain", "");
}

void handleOff() {
  ledOn = false;
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Open this address on your phone: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();   // answer browser requests
}
