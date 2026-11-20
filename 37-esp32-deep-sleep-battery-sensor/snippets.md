# Code snippets from the tutorial

These are short pieces from the article, not complete sketches. The full working sketch is [`step3/step3.ino`](step3/step3.ino).

## Step 1: timer wake-up (snippet, goes inside setup())

```cpp
esp_sleep_enable_timer_wakeup(60ULL * 1000000ULL);  // 60 seconds, in microseconds
esp_deep_sleep_start();
```

## Step 2: keep a counter across deep sleep (snippet, goes at the top of your sketch)

```cpp
RTC_DATA_ATTR int bootCount = 0;
```
