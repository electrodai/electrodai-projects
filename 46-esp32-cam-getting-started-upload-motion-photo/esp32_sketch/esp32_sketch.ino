// ESP32-CAM PIR motion photo to SD card - compiles, not yet tested on hardware
#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include "driver/rtc_io.h"

// AI-Thinker camera pins
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

RTC_DATA_ATTR int photoCount = 0;   // survives deep sleep

bool startCamera() {
  camera_config_t c = {};
  c.ledc_channel = LEDC_CHANNEL_0;
  c.ledc_timer = LEDC_TIMER_0;
  c.pin_d0 = Y2_GPIO_NUM; c.pin_d1 = Y3_GPIO_NUM;
  c.pin_d2 = Y4_GPIO_NUM; c.pin_d3 = Y5_GPIO_NUM;
  c.pin_d4 = Y6_GPIO_NUM; c.pin_d5 = Y7_GPIO_NUM;
  c.pin_d6 = Y8_GPIO_NUM; c.pin_d7 = Y9_GPIO_NUM;
  c.pin_xclk = XCLK_GPIO_NUM; c.pin_pclk = PCLK_GPIO_NUM;
  c.pin_vsync = VSYNC_GPIO_NUM; c.pin_href = HREF_GPIO_NUM;
  c.pin_sccb_sda = SIOD_GPIO_NUM; c.pin_sccb_scl = SIOC_GPIO_NUM;
  c.pin_pwdn = PWDN_GPIO_NUM; c.pin_reset = RESET_GPIO_NUM;
  c.xclk_freq_hz = 20000000;
  c.pixel_format = PIXFORMAT_JPEG;
  c.frame_size = FRAMESIZE_SVGA;     // 800x600
  c.jpeg_quality = 12;               // lower number = better quality
  c.fb_count = 1;
  return esp_camera_init(&c) == ESP_OK;
}

void setup() {
  Serial.begin(115200);
  rtc_gpio_hold_dis(GPIO_NUM_4);     // release the flash pin after sleep

  if (!startCamera()) { Serial.println("Camera init failed"); }
  else if (!SD_MMC.begin("/sdcard", true)) { Serial.println("SD failed"); } // true = 1-bit mode
  else {
    delay(500);                      // let auto exposure settle
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) {
      photoCount++;
      String path = "/motion" + String(photoCount) + ".jpg";
      File f = SD_MMC.open(path, FILE_WRITE);
      if (f) { f.write(fb->buf, fb->len); f.close(); Serial.println("Saved " + path); }
      esp_camera_fb_return(fb);
    }
  }

  // keep the flash LED off during sleep
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  rtc_gpio_hold_en(GPIO_NUM_4);

  pinMode(13, INPUT);
  while (digitalRead(13) == HIGH) delay(50); // rearm only after the PIR returns LOW
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);  // wake when PIR goes HIGH
  Serial.println("Sleeping");
  esp_deep_sleep_start();
}

void loop() {}
