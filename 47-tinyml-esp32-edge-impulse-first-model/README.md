# TinyML on ESP32 with Edge Impulse: Your First On-Device Model

Train a gesture recognition model with Edge Impulse and run it on an ESP32 with an MPU6050. Data collection, training, int8 deployment and testing.

**Full tutorial (wiring table, explanation, troubleshooting):** https://electrodai.com/tutorial/tinyml-esp32-edge-impulse-first-model

> ✅ `step1` compiles (checked with Arduino CLI, November 2026). `step2` runs your own trained model, so it needs the Arduino library you export from your Edge Impulse project (`gestures_inferencing.h`, the name depends on your project) before it can compile. Not yet tested on real hardware — [open an issue](https://github.com/electrodai/electrodai-projects/issues) if something does not work.

## Files

- [`step1/step1.ino`](step1/step1.ino)
- [`step2/step2.ino`](step2/step2.ino)

Arduino sketches: open the `.ino` file in Arduino IDE (each sits in a folder with the same name, as the IDE expects).

---
Made by [ElectroDai](https://electrodai.com) · YouTube [@electrodai](https://www.youtube.com/@electrodai) · Instagram [@electrodai.official](https://www.instagram.com/electrodai.official/)
