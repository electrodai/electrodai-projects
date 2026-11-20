# MQTT with ESP32: Send Sensor Data to Mosquitto and a Node-RED Dashboard

Publish DHT11 readings from an ESP32 over MQTT to your own Mosquitto broker, then build a Node-RED gauge and chart. Topics, wildcards and security explained.

**Full tutorial (wiring table, explanation, troubleshooting):** https://electrodai.com/tutorial/esp32-mqtt-mosquitto-node-red-dashboard

> ✅ Every sketch here compiles (checked with Arduino CLI, November 2026). Not all of them have been tested on real hardware yet — try it on your board and [open an issue](https://github.com/electrodai/electrodai-projects/issues) if something does not work.

## Files

- [`esp32_sketch/esp32_sketch.ino`](esp32_sketch/esp32_sketch.ino)

Arduino sketches: open the `.ino` file in Arduino IDE (each sits in a folder with the same name, as the IDE expects).

---
Made by [ElectroDai](https://electrodai.com) · YouTube [@electrodai](https://www.youtube.com/@electrodai) · Instagram [@electrodai.official](https://www.instagram.com/electrodai.official/)
