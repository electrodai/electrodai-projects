// ElectroDai - DHT11 weather station with a 16x2 I2C LCD
// DHT11 DATA -> D2, LCD SDA -> A4, LCD SCL -> A5

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);   // try 0x3F if blank

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();          // start the LCD
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ElectroDai");
  lcd.setCursor(0, 1);
  lcd.print("Weather Station");
  delay(2000);
  lcd.clear();
}

void loop() {
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();   // Celsius

  // A failed read returns NaN (not a number)
  if (isnan(humidity) || isnan(tempC)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor error    ");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring    ");
    Serial.println("DHT read failed");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(tempC, 1);
    lcd.print(" C   ");          // spaces clear old digits
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity, 0);
    lcd.print("%  ");
    Serial.print(tempC);
    Serial.print(" C, ");
    Serial.print(humidity);
    Serial.println(" %");
  }
  delay(2000);   // DHT11 needs 1 s or more between reads
}
